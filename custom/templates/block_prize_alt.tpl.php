<div class="daily-prize-block prize-<?php print $prize->field_category; ?>">
  <div class="container">
    <div class="row">

      <?php if ( isset( $message ) ) : ?>

        <div class="span3">
          <p class="button-status-message"><?php print $message; ?></p>
        </div>

      <?php else: ?>

        <div class="span3">
          <div class="daily-prize-description">
            <h4>Tomorrow's Daily Prize is <?php print $prize->title; ?></h4>
            <p><?php print t("Please refer to the <a href='@schedule_link'>Daily Prize Schedule</a> for a complete listing of all Daily Prizes.", array("@schedule_link" => 'prizes/daily' )); ?></p>
          </div>
        </div>

        <div class="span3">
          <div class="daily-prize-img">
            <img src="<?php print $prize_image; ?>", 'alt'="<?php print $prize->title; ?>" />
          </div>
        </div>

      <?php endif; ?>

    </div>
  </div>
</div>