# Quiz Module: Backdrop Architecture and Porting Notes

This document explains how the Quiz module works in this Backdrop repository and
what was changed to port it from the Drupal 7 implementation.

## Scope

- Source reference used during porting: `files/coder_upgrade/old/quiz`
- Active module path: `modules/contrib/quiz`
- This doc focuses on runtime behavior, data model, and port-specific changes.

## How the module works

### Primary content model

- A quiz is stored as a node of type `quiz`.
- Questions are stored as nodes, one node type per question module
  (`multichoice`, `truefalse`, `matching`, etc.).
- Quiz settings and relationships are stored in custom entity-backed tables.

### Custom entities used by Quiz

The module defines and uses these entity types:

- `quiz` (table: `quiz_node_properties`)
- `quiz_question_relationship` (table: `quiz_node_relationship`)
- `quiz_result` (table: `quiz_node_results`)
- `quiz_result_answer` (table: `quiz_node_results_answers`)
- `quiz_result_type` (table: `quiz_result_type`, bundle definition for
  `quiz_result`)

These are declared in `quiz_entity_info()` in `quiz.module`.

### Main request flow

### Authoring

- Quiz/question editing is node-based.
- Additional quiz/question properties are loaded through entity controllers and
  question-type classes.
- `quiz_question` provides the shared question API and base class behavior for
  all question type modules.

### Taking a quiz

- Access and start logic is handled in `quiz.module` and `quiz.pages.inc`.
- The current attempt (`quiz_result`) tracks attempt-level state.
- Each answered question is stored as `quiz_result_answer`.
- Scoring uses a mix of auto-scoring by question type and optional manual
  scoring workflows.

### Review/reporting

- Result rendering uses entity view/controller output.
- Feedback visibility is controlled by quiz settings, attempt state, and
  question-level review logic.
- Default Views are provided for result/admin reporting and question banks.

### Question type plugin pattern

- Each question type module registers metadata via `hook_quiz_question_info()`.
- The shared `quiz_question` module resolves that metadata into:
  - a question class (extends `QuizQuestion`)
  - a response class (extends `QuizQuestionResponse`)
- Question modules own their own storage tables for type-specific options and
  answer payloads.

## Backdrop port changes

### Dependency and metadata updates

- `.info` files were converted from Drupal 7 metadata to Backdrop format
  (`backdrop = 1.x`, `type = module`).
- Core dependencies shifted to Backdrop equivalents:
  - `entity` -> `entity_plus` + `entity_ui`
  - `ctools` plugin registration -> `plugin_manager`

Current root dependencies are in `quiz.info`.

### Entity layer migration

- Controllers migrated from `EntityAPIController` to `EntityPlusController`.
- Views controllers migrated to `EntityPlusDefaultViewsController`.
- Exportable result-type controller migrated to `EntityPlusControllerExportable`.
- Entity classes were made explicit where Backdrop expects `EntityInterface`
  methods (`id()`, `entityType()`, `uri()`), including:
  - `Quiz`
  - `QuizResult`
  - `QuizResultAnswer`
  - `QuizQuestionRelationship`
  - `QuizResultType`
  - `QuizQuestionEntity` (for `quiz_question`)

### Compatibility helpers added in `quiz.module`

- `entity_save()` wrapper (when not already defined)
- `entity_load_single()` wrapper (when not already defined)
- `_quiz_entity_view_extract()` helper for normalizing render-array structures
  across D7-style and Backdrop-style entity view output
- `quiz_result_label()` callback for `quiz_result` label handling

These helpers reduce changes across legacy business logic that still calls
Entity API-style functions.

### Autoload and asset loading updates

- Replaced D7 `files[]` registration with `hook_autoload_info()` mappings.
- Replaced stylesheet declaration in `.info` with Backdrop library registration:
  - `hook_library_info()`
  - `hook_init()` calls `backdrop_add_library('quiz', 'styles')`

### Install/update hardening for Backdrop config entities

Install and updates now ensure:

- `quiz` node type exists as a Backdrop config entity
- body field is added only when missing
- default `quiz_result_type` exists via `db_merge()`
- default quiz Views are installed into config when missing

See `quiz_install()` and update hooks `quiz_update_7526()` to
`quiz_update_7528()`.

### Question type provisioning updates

In `question_types/quiz_question/quiz_question.module`:

- Added `quiz_question_ensure_node_type()` to create missing question node types
  as Backdrop config entities.
- `quiz_question_add_body_field()` now guards against duplicate field instance
  creation and missing instance reads.
- Added module autoload mappings via `hook_autoload_info()`.

### CTools and plugin_manager status

- Backdrop plugin registration hooks were added:
  - `hook_plugin_manager_directory()`
  - `hook_plugin_manager_api()`
- Legacy `hook_ctools_plugin_directory()` and `hook_ctools_plugin_api()` remain
  for compatibility with existing plugin-style code and migration continuity.
- Some panel/page-manager integration code still uses legacy `ctools_*` symbols
  in plugin files. This is usually only relevant if those integration paths are
  enabled in a given site.

### AJAX updates

In `modules/ajax_quiz/ajax_quiz.module`:

- Replaced `ctools_ajax_command_redirect()` with
  `ajax_command_redirect(url(..., array('absolute' => TRUE)))`.
- Added a guard when reading navigation children during form alteration.

### Known migration caveat to review

In `_quiz_question_get_instance()`, question-provider lookup should guard both:

- the presence of `$node->type`, and
- the presence of a corresponding key in `quiz_question_get_info()`.

If only `$node->type` is checked, unknown types can still trigger undefined
index notices.

### Operational notes for this repository

- Use DDEV and Bee for module operations:
  - `ddev bee projects quiz`
  - `ddev bee cache-clear all`
  - `ddev bee update-db`
- For PHP changes, lint modified files:
  - `ddev php -l modules/contrib/quiz/quiz.module`
