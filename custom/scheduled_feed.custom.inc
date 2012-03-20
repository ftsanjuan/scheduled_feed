<?php

/**
 * @file
 * Support functions for hooking into other modules and providing
 * custom functionality
 */

/**
 * Implements hook_framework_game_match_swf()
 *
 * Adds flashvar for retrieving a daily recipe.
 */
function scheduled_feed_framework_game_match_swf(&$swf) {
  $swf->flashVar('recipeID', scheduled_feed_get());
}