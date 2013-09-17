<?php
/**
 * @file
 * show link to download PDF.
 */
?>

<?php if (isset($variables['pdf_download_path'])): ?>
  <div style="float:left; position:relative; left:20px">
    <a href = '<?php print $variables['pdf_download_path'] ?>'>
      <img src = "<?php print $variables['pdf_download_image'] ?>" /></a>
  </div>
  <div style="position:relative; left:35px">
    <a href = '<?php print $variables['pdf_download_path'] ?>'><br/>Download</a>
  </div>
<?php endif; ?>