<?php

/**
 * Provides core functionality for Scheduled Feed
 */

define('SCHEDULED_FEED_CURRENT_TIMEZONE', '-0500');

define('SCHEDULED_FEED_FORM_TIME_INPUT_FORMAT', 'Y-M-j g:i:s a');

/**
 * Utility : Returns a timezone-corrected timestamp for the supplied Drupal date array
 */
function scheduled_feed_date_array_to_timestamp($date) {
  $date = strtotime($date . ' ' . SCHEDULED_FEED_CURRENT_TIMEZONE);
  return scheduled_feed_timezone_dst_corrected_timestamp($date);
}

/**
 * Utility : Returns the starting timestamp (start of day) for a particular timestamp
 */
function scheduled_feed_day_start_timestamp($timestamp = NULL) {
  // default to current timestamp
  $timestamp = is_null($timestamp) ? time() : $timestamp;

  // get just the day string for this timestamp
  // and let strtotime resolve the start timestamp
  $day = date('F j Y', $timestamp);
  $start = strtotime($day . ' ' . SCHEDULED_FEED_CURRENT_TIMEZONE);
  $start = scheduled_feed_dst_corrected_timestamp($start);

  return $start;
}

/**
 * Returns a timestamp corrected for daylight savings particular timezone
 */
function scheduled_feed_dst_corrected_timestamp($timestamp) {
   // calculate for daylight savings
  if ( date("I", $timestamp) ) {
    // subtract 3600 seconds (= 1 hour)
    $timestamp -= 3600;
  }
  return $timestamp;
}