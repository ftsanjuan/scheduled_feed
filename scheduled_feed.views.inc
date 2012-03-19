<?php

/**
 * Implements hook_views_data().
 */
function scheduled_feed_views_data() {
  $data['scheduled_feed_data']['table']['group'] = t('Scheduled Feed');
  $data['scheduled_feed_data']['table']['base'] = array(
    'field' => 'id',
    'title' => t('Scheduled Feed'),
    'help' => t('Scheduled Feed Data.'),
    'access query tag' => 'user_access',
  );

  $data['scheduled_feed_data']['id'] = array(
    'title' => t('Data ID'),
    'help' => t('Data ID'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );

  $data['scheduled_feed_data']['tid'] = array(
    'title' => t('Tid'),
    'help' => t('Group/Category ID'),
    'relationship' => array(
      'base' => 'taxonomy_term_data',
      'field' => 'tid',
      'handler' => 'views_handler_relationship',
      'label' => t('Term ID'),
    ),
  );

  $data['scheduled_feed_data']['data'] = array(
    'title' => t('Data'),
    'help' => t('The data to be displayed.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['scheduled_feed_data']['start'] = array(
    'title' => t('Start Timestamp'),
    'help' => t('Start Timestamp'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $data['scheduled_feed_data']['end'] = array(
    'title' => t('End Timestamp'),
    'help' => t('End Timestamp'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $data['scheduled_feed_data']['is_default'] = array(
    'title' => t('Is Default?'),
    'help' => t('Is default?'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
    ),
  );
  return $data;
}