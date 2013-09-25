<?php
/**
 * @file
 * Load the google reader via an iframe.
 */
?>

<?php if ($variables['show_viewer']): ?>
  <div class="islandora-content-wrapper">
    <div class="islandora-google-pdfreader-iframe-wrapper">
      <iframe src="<?php print $variables['viewer_url']; ?>"></iframe>
        <?php if (isset($variables['pdf_download_path'])): ?>
        <?php endif; ?>
    </div>
</div>
<?php endif; ?>

<?php if (isset($variables['pdf_download_path'])): ?>
  <br/>
  <div class="islandora-download-image-wrapper">
    <a href="<?php print $variables['pdf_download_path'] ?>">
      <img src="<?php print $variables['pdf_download_image'] ?>"/></a>
  </div>
  <div class="islandora-download-text-wrapper">
    <a href="<?php print $variables['pdf_download_path'] ?>"><br/>Download</a>
  </div>
<?php endif; ?>
