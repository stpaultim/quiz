<?php

class QuizResultAnswer extends Entity {

  /**
   * Implements EntityInterface::id().
   */
  public function id() {
    return isset($this->result_answer_id) ? $this->result_answer_id : NULL;
  }

  /**
   * Implements EntityInterface::entityType().
   */
  public function entityType() {
    return 'quiz_result_answer';
  }

  /**
   * Legacy Entity API compatibility.
   */
  public function identifier() {
    return $this->id();
  }

  /**
   * Implements EntityInterface::uri().
   */
  public function uri() {
    return array();
  }
}
