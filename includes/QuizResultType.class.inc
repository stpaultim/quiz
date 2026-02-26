<?php

class QuizResultType extends Entity {

  /**
   * Implements EntityInterface::id().
   */
  public function id() {
    return isset($this->id) ? $this->id : NULL;
  }

  /**
   * Implements EntityInterface::entityType().
   */
  public function entityType() {
    return 'quiz_result_type';
  }

  /**
   * Implements EntityInterface::uri().
   */
  public function uri() {
    if (!empty($this->type)) {
      return array(
        'path' => 'admin/quiz/result/manage/' . $this->type,
      );
    }

    return array();
  }
}
