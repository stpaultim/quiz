<?php

class QuizQuestionEntity extends Entity {

  /**
   * Implements EntityInterface::id().
   */
  public function id() {
    return isset($this->qqp_id) ? $this->qqp_id : NULL;
  }

  /**
   * Implements EntityInterface::entityType().
   */
  public function entityType() {
    return 'quiz_question';
  }

  /**
   * Implements EntityInterface::uri().
   */
  public function uri() {
    return array();
  }
}
