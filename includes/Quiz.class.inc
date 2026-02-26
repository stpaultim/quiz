<?php

class Quiz extends Entity {

  /**
   * Implements EntityInterface::id().
   */
  public function id() {
    return isset($this->qnp_id) ? $this->qnp_id : NULL;
  }

  /**
   * Implements EntityInterface::entityType().
   */
  public function entityType() {
    return 'quiz';
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
