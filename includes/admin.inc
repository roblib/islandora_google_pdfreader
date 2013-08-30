<?php

/**
 * @file
 * Admin callbacks for Islandora Google PDF Reader
 *
 */
function islandora_google_pdfreader_admin() {
  $form = array();
  $coptions = islandora_google_pdfreader_get_content_models();
  $selected = variable_get('islandora_google_pdfreader_mappings', array());
  
  $chosen = array_keys($selected);
  $pids = array_keys($coptions);
  $alink_options = array(
    'attributes' => array('target' => '_blank'),
    'html' => TRUE,
  );
  foreach ($selected as $key => $value) {
    $label = isset($coptions[$key]) ? $coptions[$key] : "Unlabeled";
    unset($coptions['$key']);
    $coptions = array($key => $label) + $coptions;
  }

 

  $form['content_model_wrapper'] = array(
    '#type' => 'fieldset',
    '#title' => t('Select Content Models'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['content_model_wrapper']['pdf_table'] = array(
    '#type' => 'item',
    '#title' => t('Select Content Models for the Google Pdf Reader'),
    '#description' => t('Content Models selected here will add a tab for the Google Pdf Reader.'),
    '#tree' => TRUE, // this attribute is important to return the submitted values in a deeper nested arrays in
    '#theme' => 'islandora_google_pdfreader_admin_table',
  );

  foreach ($coptions as $pid => $label) {
    $dsids = islandora_google_pdfreader_get_dsids_from_dscomp($pid);
    if (empty($dsids)) {
      continue;
    }
    $options[$pid] = '';
    // label
    $form['content_model_wrapper']['pdf_table']['name'][$pid] = array(
      '#type' => 'item',
      '#markup' => l($label, "islandora/object/$pid", $alink_options),
    );
    // pid
    $form['content_model_wrapper']['pdf_table']['content_model'][$pid] = array(
      '#type' => 'item',
      '#markup' => $pid,
    );

    // dsid


    $form['content_model_wrapper']['pdf_table']['DSID'][$pid] = array(
      '#prefix' => '<div class="dsidSelect">',
      '#suffix' => '</div>',
      '#type' => 'select',
      '#options' => $dsids,
      '#default_value' => isset($selected[$pid]['DSID']) ? $selected[$pid]['DSID'] : '',
    );    

    $form['content_model_wrapper']['pdf_table']['enabled'] = array(
      '#type' => 'checkboxes',
      '#options' => $options,
      '#default_value' => $chosen,
    );
  }

 
  $form['actions'] = array(
    '#type' => 'actions'
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Configuration'),
    '#weight' => 0,
    '#submit' => array('islandora_google_pdfreader_admin_submit'),
  );


  return $form;
}

/**
 * NEED VALIDATION 
 * @param type $form
 * @param type $form_state 
 */
function islandora_google_pdfreader_admin_submit($form, &$form_state) {
  $enabled = array_filter($form_state['values']['pdf_table']['enabled']);
  $content_model_mappings = array();
  foreach ($enabled as $label => $pid) {
    $content_model_mappings[$pid]['DSID'] = $form_state['values']['pdf_table']['DSID'][$pid];
  }
  // variable_set('islandora_image_annotation_djatoka_path',$form_state['values']['islandora_image_annotation_djatoka_path']);
  variable_set('islandora_google_pdfreader_mappings', $content_model_mappings);
 
}

/**
 * 
 * themes the form table.
 *
 * @param array $element Drupal Form Element.
 * @return string
 */
function theme_islandora_google_pdfreader_admin_table($variables) {
  // set form
  $form = $variables['form'];
  $rows = array();

  foreach ($form['name'] as $key => $element) {
    // Do not take form control structures.
    if (is_array($element) && element_child($key)) {
      $row = array();

      $row[] = array('data' => drupal_render($form['enabled'][$key]));
      $row[] = array('data' => drupal_render($form['name'][$key]), 'title' => $key);
      $row[] = array('data' => drupal_render($form['content_model'][$key]));
      $row[] = array('data' => drupal_render($form['DSID'][$key]));      

      // add to rows
      $rows[] = array('data' => $row, 'class' => array('draggable'));
    }
  }

  // Individual table headers.
  $header = array();

  $header[] = array('data' => t('Enable Annotating'));
  $header[] = array('data' => t('Content Model'));
  $header[] = array('data' => t('PID'));
  $header[] = array('data' => t('DSID'));
  

  // render form
  $output = '';
  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'islandora_google_pdfreader_admin_table')));
  $output .= drupal_render_children($form);
  return $output;
}

function islandora_google_pdfreader_get_content_models() {
  module_load_include('inc', 'islandora', 'includes/tuque');
  $repository = new IslandoraTuque();
  $query = 'select $object $title from <#ri>
               where $object <info:fedora/fedora-system:def/model#hasModel> <info:fedora/fedora-system:ContentModel-3.0>
               and $object <fedora-model:label> $title
               and ($object <fedora-view:disseminates> $ds1 in <#ri>
               and $ds1 <fedora-view:disseminationType> <info:fedora/*/DS-COMPOSITE-MODEL> in <#ri>)
               order by $title';
  $objects = $repository->repository->ri->itqlQuery($query, 'unlimited', '0');
  $return_array = array();
  foreach ($objects as $object) {
    $cmodel_pid = $object['object']['value'];
    if (strpos($cmodel_pid, 'fedora-system') !== 0) {
      $cmodel_title = $object['title']['value'];
      if ($cmodel_title) {
        $title_string = $cmodel_title . ' ~ ' . $cmodel_pid;
      }
      else {
        $title_string = $cmodel_pid;
      }

      $return_array[$cmodel_pid] = $cmodel_title;
    }
  }
  return $return_array;
}

function islandora_google_pdfreader_get_dsids_from_dscomp($pid) {
  module_load_include('inc', 'islandora', 'includes/tuque');
  module_load_include('inc', 'islandora', 'includes/utilities');
  $fedora_object = islandora_object_load($pid);
  $dsids = islandora_get_datastreams_requirements_from_content_model($fedora_object);
  $output = array();
  foreach($dsids as $dsid){
    if(strstr($dsid['mime'][0], 'pdf')){
      $output [(string)$dsid['id']] = (string) $dsid['id'];
    }
  }
  return $output;
}

