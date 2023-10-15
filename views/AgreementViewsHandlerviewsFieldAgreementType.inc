<?php
/**
 * @file
 * Definition of AgreementViewsHandlerviewsFieldAgreementType.
 */

/**
 * Field handler to translate an agreement type into its readable form.
 */
class AgreementViewsHandlerviewsFieldAgreementType extends views_handler_field {
  function option_definition() {
    $options = parent::option_definition();
    $options['machine_name'] = array('default' => FALSE, 'bool' => TRUE);

    return $options;
  }

  /**
   * Provide machine_name option for agreement type.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['machine_name'] = array(
      '#title' => t('Output machine name'),
      '#description' => t('Display field as the agreement type machine name.'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['machine_name']),
    );
  }

  /**
    * Render node type as human readable name, unless using machine_name option.
    */
  function render($values) {
    $value = $this->get_value($values);
    if ($this->options['machine_name'] != 1 && $value !== '') {
      return t($this->sanitize_value(agreement_type_options($value)));
    }
    return $this->sanitize_value($value);
  }
}
