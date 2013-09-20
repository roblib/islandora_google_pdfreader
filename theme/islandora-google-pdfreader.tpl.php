<?php
/**
 * @file
 * load the google reader via an iframe.
 */
?>

<?php if ($variables['show_viewer']): ?>
<div class="islandora-content-wrapper">
  <div class="islandora-google-pdfreader-iframe-wrapper">
    <table>
      <tr>
        <td style=width:50%>
          <iframe src="<?php print $variables['viewer_url']; ?>" style="width:600px; height:600px;" frameborder="0"></iframe>
        </td>
        <?php if (isset($variables['pdf_download_path'])): ?>
          <td style="width:50%; vertical-align:top;">
        <?php endif; ?>
    </div>
</div>
<br/>
<?php endif; ?>

<?php if (isset($variables['pdf_download_path'])): ?>
  <div style="float:left; position:relative; left:20px;">
    <a href="<?php print $variables['pdf_download_path'] ?>">
      <img src="<?php print $variables['pdf_download_image'] ?>"/></a>
  </div>
  <div style="position:relative; left:35px">
    <a href="<?php print $variables['pdf_download_path'] ?>"><br/>Download</a>
  </div>
<?php endif; ?>
</td></tr></table>