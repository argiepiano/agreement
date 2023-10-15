<?php
/**
 * @file
 * Definition of AgreementViewsHandlerviewsFilterAgreementType.
 */

/**
 * Filter by agreement type.
 */
class AgreementViewsHandlerviewsFilterAgreementType extends views_handler_filter_in_operator {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Agreement types');
      $options = agreement_type_options();
      asort($options);
      $this->value_options = $options;
    }
  }
}
