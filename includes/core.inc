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

  // calculate for daylight savings
  if ( date("I", $date) ) {
    // subtract 3600 seconds (= 1 hour)
    $date -= 3600;
  }
  return $date;
}