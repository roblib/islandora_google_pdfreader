<?php
/**
 * @file
 * show link to download PDF.
 */
?>

<?php if (isset($variables['pdf_download_path'])): ?>
  <?php drupal_add_css(drupal_get_path('module', 'islandora_google_pdfreader') . '/css/islandora_google_pdfreader.css'); ?>
  <br/>
  <div id="pdf-download-table">
    <table class="noborder">
      <tr></tr>
      <tr>
        <td class="pdf-download-image-cell">
          <a href = '<?php print $variables['pdf_download_path'] ?>'>
            <img src = "<?php print $variables['pdf_download_image'] ?>" /></a>
        </td>
        <td class="pdf-download-text-cell">
          <a href = '<?php print $variables['pdf_download_path'] ?>'>Download</a>
        </td>
      </tr>
    </table>
  </div>
<?php endif; ?>