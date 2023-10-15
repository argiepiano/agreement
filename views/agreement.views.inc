<?php

/**
 * @file
 * Agreement views info.
 */

/**
 * Implements hook_views_data().
 */
function agreement_views_data() {
  $data = array();
  $data['agreement']['table']['group'] = t('Agreement');

  $data['agreement']['table']['base'] = array(
    'field' => 'id',
    'title' => t('Agreement'),
    'weight' => -10,
    'defaults' => array(
      'field' => 'id',
    ),
  );

  $data['agreement']['table']['join'] = array(
    'users' => array(
      'left_field' => 'uid',
      'field' => 'uid',
      'type' => 'left',
    ),
  );

  $data['agreement']['id'] = array(
    'title' => t('Agreement ID'),
    'help' => t('The agreement ID.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
      'float' => FALSE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'numeric' => TRUE,
      'validate type' => 'id',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['agreement']['uid'] = array(
    'title' => t('Account ID'),
    'help' => t('The user that has agreed.'),
    'relationship' => array(
      'title' => t('User account'),
      'help' => t('Relate content to the user who agreed to it.'),
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'field' => 'uid',
      'label' => t('author'),
      'relationship field' => 'uid',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_user_name',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'field' => array(
      'handler' => 'views_handler_field_user',
    ),
  );

  $data['agreement']['type'] = array(
    'title' => t('Type'),
    'help' => t('The agreement type.'),
    'field' => array(
      'handler' => 'views_handler_field_machine_name',
      'click sortable' => TRUE,
      'options callback' => array(
        'agreement_type_options'
      ),
    ),
    // 'sort' => array(
    //   'handler' => 'views_handler_sort',
    // ),
    // 'filter' => array(
    //   'handler' => 'teacher_handler_filter_teacher_type',
    // ),
    // 'argument' => array(
    //   'handler' => 'views_handler_argument_string',
    // ),
  );
  
  $data['agreement']['agreed_date'] = array(
    'title' => t('Agreement date'),
    'help' => t('The date the agreement was submitted.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
  );

  return $data;
}
