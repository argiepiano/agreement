<?php

/**
 * @file
 * Agreement page callbacks.
 */

/**
 * Callback for agreement URL.
 *
 * @param string $agreement_type
 *   The agreement type name.
 *
 * @return string
 *   HTML output.
 *
 * @throws \Exception
 *   Wrapped exception from theme().
 */
function agreement_page($agreement_type) {
  global $user;

  // Redirect anonymous users to the home page.
  if (!$user->uid) {
    backdrop_goto('<front>');
  }

  $info = agreement_type_load($agreement_type);
  $status = agreement_has_agreed($user, $info);

  $form = backdrop_get_form('agreement_form', $status, $info, $user);
  $output = theme('agreement_page', array('form' => $form));
  return $output;
}

/**
 * FAPI definition for the agreement form.
 *
 * @param array $form
 *   The form array.
 * @param array &$form_state
 *   The form state array.
 * @param bool $status
 *   Whether the user has agreed or not.
 * @param array $info
 *   The agreement type.
 * @param object $account
 *   The user account.
 *
 * @return array
 *   The form array.
 *
 * @ingroup forms
 *
 * @see agreement_form_validate()
 * @see agreement_form_submit()
 */
function agreement_form($form, &$form_state, $status, $info, $account) {

  $form['text'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('agreement-text'),
    ),
    'terms' => array(
      '#markup' => check_markup($info['agreement'], $info['settings']['format']),
    ),
  );

  $form_state['account'] = $account;
  $form_state['info'] = $info;
  $form_state['agreed'] = $status;
  $can_revoke = user_access('revoke own agreement');

  // Only display the agree checkbox and submit button when the user needs to
  // agree. All other users may only see the agreement.
  if ((!$status || $can_revoke) &&
      !user_access('bypass agreement') &&
      _agreement_user_has_role($account, $info['settings']['role'])) {
    $form['agree'] = array(
      '#default_value' => $status,
      '#title' => check_plain($info['settings']['checkbox']),
      '#type' => 'checkbox',
    );

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => check_plain($info['settings']['submit']),
    );
  }

  return $form;
}

/**
 * Validation handler for agreement_form().
 *
 * @ingroup forms
 *
 * @see agreement_form()
 * @see agreement_form_submit()
 */
function agreement_form_validate($form, &$form_state) {
  if (!$form_state['values']['agree'] && !$form_state['agreed']) {
    form_set_error('agree', check_plain($form_state['info']['settings']['failure']));
  }
}

/**
 * Submit handler for agreement_form().
 *
 * @ingroup forms
 *
 * @see agreement_form()
 * @see agreement_form_validate()
 */
function agreement_form_submit($form, &$form_state) {
  $account = $form_state['account'];
  $info = $form_state['info'];
  $uid = $account->uid;
  $agree = $form_state['values']['agree'];
  $had_agreed = $form_state['agreed'];

  try {
    $transaction = db_transaction();
    $mail_key = $agree ? 'notice' : 'revoked';
    $message = $agree ? $info['settings']['success'] : $info['settings']['revoked'];
    agreement_agree($account, $info['name'], $agree);
    backdrop_set_message(check_plain($message));

    // Send email notifying of acceptance if admin has set an email address.£
    if ($info['settings']['email_recipient'] && (!$had_agreed || !$agree)) {
      $params['account'] = $account;
      backdrop_mail('agreement', $mail_key, $info['settings']['email_recipient'], user_preferred_language($account), $params);
    }

    $agreement_success_destination = str_replace(
      '&lt;front&gt;',
      '<front>',
      check_plain($info['settings']['destination'])
    );

    if (isset($_SESSION['agreement_destination']) && $_SESSION['agreement_destination'] === 'change password') {
      // Always go to the user account edit page if original destinatino was
      // user/reset.
      $redirect = 'user/' . $uid . '/edit';
    }
    elseif (!$agreement_success_destination) {
      $redirect = isset($_SESSION['agreement_destination']) ? $_SESSION['agreement_destination'] : '<front>';
    }
    else {
      $redirect = $agreement_success_destination;
    }

    unset($_SESSION['agreement_destination']);
    $form_state['redirect'] = $redirect;
  }
  catch (\PDOException $e) {
    $transaction->rollback();
    backdrop_set_message(t('An error occurred accepting the agreement.'), 'error');
  }
}
