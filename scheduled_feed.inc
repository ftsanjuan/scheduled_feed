<?php

/**
 * @file
 * Supporting functions for adding/retrieving/setting default dates
 */

/**
 * Adds a piece of data to be returned between a particular
 * start timestamp and end timestamp
 *
 *  @param $data
 *    the data string to be returned within the given period
 *  @param $start
 *    the start timestamp for when this data should be returned
 *  @param $end
 *    the end timestamp for when this data should stop being returned
 *  @param $is_default
 *    if TRUE, sets this data to be default shown when no data
 *    could be found for the current time
 *  @param $tid
 *    the group/category this data belongs to
 *    Default: Primary Feed's tid
 */
function scheduled_feed_data_add($data, $start, $end, $is_default = FALSE, $tid = NULL) {
  if ( is_null($tid) ) {
    $tid = scheduled_feed_get_primary_tid();
  }

  $data = (object) array(
    'data' => $data,
    'start' => $start,
    'end' => $end,
    'is_default' => $is_default,
    'tid' => $tid,
  );
  scheduled_feed_data_save($data);
}

/**
 * Saves a feed data record.
 *
 *  @param $data
 *  a feed data object
 */
function scheduled_feed_data_save($data) {
  drupal_write_record('scheduled_feed_data', $data);
  dsm('Feed data was saved successfully.');
  return $data;
}

/**
 * Clears the feed data table
 */
function scheduled_feed_data_clear() {
  db_truncate('scheduled_feed_data')
    ->execute();
  dsm(t('Scheduled feed data cleared successfully.'));
}

/**
 * Returns the data for a particular timestamp
 *
 * @param $time
 *   the timestamp to check for
 *   default: time()
 * @param $tid
 *   the group/category to check in
 *   Default: Primary Feed's tid
 */
function scheduled_feed_get($time = NULL, $tid = NULL) {
  if ( is_null($time) ) {
    $time = time();
  }

  if ( is_null($tid) ) {
    $tid = scheduled_feed_get_primary_tid();
  }

  $feed = db_select('scheduled_feed_data', 'd')
    ->fields('d')
    ->condition('start', $time, '<=')
    ->condition('end', $time, '>')
    ->condition('tid', $tid, '=')
    ->execute()
    ->fetch();

  if ( $feed ) {
    return $feed->data;
  }
  else {
    // try to get the default data if it was previously defined
    $default = scheduled_feed_get_default($tid);
    if ( $default ) {
      return $default;
    }
  }

  dsm(t('No data was found.'));
  return FALSE;
}

/**
 * Returns the data for a particular timestamp listed under
 * a specified feed name
 *
 * @param $name
 *   the name of the feed to retrieve data from
 * @param $time
 *   the timestamp to check for
 *   default: time()
 */
function scheduled_feed_get_by_feed_name($name, $time = NULL) {
  $term = taxonomy_get_term_by_name($name);

  if ( count($term) > 0 ) {
    // retreive the first term in the array
    // recall: results from taxonomy_get_term_by_name
    // are indexed by their tid
    foreach ( $term as $key => $value ) {
      $tid = $key;
      break;
    }

    if ( is_null($time) ) {
      $time = time();
    }

    $feed = db_select('scheduled_feed_data', 'd')
      ->fields('d')
      ->condition('start', $time, '<=')
      ->condition('end', $time, '>')
      ->condition('tid', $tid, '=')
      ->execute()
      ->fetch();

    if ( $feed ) {
      return $feed->data;
    }
    else {
      // try to get the default data if it was previously defined
      $default = scheduled_feed_get_default($tid);
      if ( $default ) {
        return $default;
      }
    }
  }

  dsm(t('No data was found.'));
  return FALSE;
}

/**
 * Returns the default data if it is defined
 *
 * @param $tid
 *    the group/category id to retrieve data from
 *    Default: Primary Feed's tid
 */
function scheduled_feed_get_default($tid = NULL) {
  if ( is_null($tid) ) {
    $tid = scheduled_feed_get_primary_tid();
  }

  $default = db_select('scheduled_feed_data', 'd')
    ->fields('d')
    ->condition('tid', $tid, '=')
    ->condition('is_default', TRUE, '=')
    ->execute()
    ->fetch();

  if ( $default ) {
    return $default->data;
  }

  dsm(t('No default data was defined.'));
  return FALSE;
}

/**
 * Sets a particular piece of feed data to be the default
 *  data returned
 *
 * @param $id
 *  The id of the piece of feed data
 */
function scheduled_feed_set_default($id) {
  $result = db_update('scheduled_feed_data')
    ->fields(array(
        'is_default' => TRUE
      ))
    ->condition('id', $id, '=')
    ->execute();

  if ( $result ) {
    dsm(t('Data with id @id was successfully set as default.', array('@id' => $id)));
  }

  dsm(t('Default data could not be set.'));
  return FALSE;
}

/**
 * Verifies if there are any overlapping pieces of data in a particular feed
 *
 * @param $tid
 *    the id of the feed to verify
 *    Default: Primary Feed's tid
 *  @return
 *    TRUE if there are no issues with the particular feed, FALSE otherwise
 */
function scheduled_feed_verify($tid = NULL) {
  if ( is_null($tid) ) {
    $tid = scheduled_feed_get_primary_tid();
  }

  $feed = db_select('scheduled_feed_data', 'd')
    ->fields('d')
    ->condition('tid', $tid, '=')
    ->orderBy('start')
    ->execute()
    ->fetchAll();

  if ( $feed ) {
    // iterate through results to check for overlaps
    for ( $i = 0; $i < count($feed) - 1; $i++ ) {
      if ( $feed[$i]->start < $feed[$i+1]->end || $feed[$i]->end > $feed[$i+1]->start ) {
        dsm(t('Overlapping periods detected for: @data1 and @data2', array('@data1' => $feed[$i]->data, '@data2' => $feed[$i+1]->data)));
        return FALSE;
      }
    }
    return TRUE;
  }
}

/**
 * Returns the vocabulary id containing the Scheduled Feed
 * groups/categories
 */
function scheduled_feed_get_vid() {
  return variable_get('scheduled_feed_vid', FALSE);
}

/**
 * Returns the primary scheduled feed's tid
 */
function scheduled_feed_get_primary_tid() {
  return variable_get('scheduled_feed_primary_tid', FALSE);
}

/**
 * Generates a feed from an array of strings and separates them by
 * a specified period length
 *
 * @param $data
 *    An array of strings containing the data to be displayed
 * @param $start
 *    Timestamp when the first data will start being displayed
 *    Default: time()
 * @param $length
 *    The length of time (in seconds), to display each piece of data
 *    Default: 60
 * @param $tid
 *    The feed id this data should be associated with
 *    Default: (scheduled feed primary tid)
 * @param $clear
 *    if TRUE, clears all feed data already in database before
 *      adding new feed data
 *    Default: TRUE
 */
function scheduled_feed_generate_sequential_feed($data, $start = NULL, $length = 60, $tid = NULL, $clear = TRUE) {
  if ( $clear ) {
    scheduled_feed_data_clear();
  }

  if ( is_null($start) ) {
    $start = time();
  }

  if ( is_null($tid) ) {
    $tid = scheduled_feed_get_primary_tid();
  }

  for ( $i = 0; $i < count($data); $i++ ) {
    scheduled_feed_data_add($data[$i], $start + ($i * $length), $start + (($i + 1) * $length), FALSE, $tid);
  }

  dsm(t('Successfully generated feed.'));
}