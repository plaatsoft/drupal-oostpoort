<?php

/* 
**  Church Administration (oostpoort) module for drupal 
**
**  Copyright (C) 2009-2010
**  =======================
**
**  Created by wplaat
**
**  For more information visit the following website.
**  Website : http://www.plaatsoft.nl 
**
**  Or send an email to the following address.
**  Email   : info@plaatsoft.nl
**
**  This program is free software; you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, version 2.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

// ##################################################################################
// CSV FILE UPLOAD 
// ##################################################################################

/**
 * process csv upload file.
 * @return errors in HTML format
 */
function oostpoort_process_csv_upload() {

  global $user;

  // Breadcrumb menu
  $page  = '<div class="breadcrumb">';
  $page .= '<a href="'.url("").'">Home</a> > ';
  $page .= '<a href="'.url(OOSTPOORT_MENU).'">Menu</a> > ';
  $page .= 'Upload CSV';
  $page .= '</div>';

  $page .= '<div class="oostpoort">';
  $page .= '<br/>';

  $line_counter=0;
  $csv_file_error=0;

  $myFile = 'files/upload.csv';
  $fh = fopen($myFile, 'r');

  // First delete all old information
  $query='delete from oostpoort_members';
  $queryResult = db_query($query);

  // Header line, skip it!
  $theData = fgets($fh);
  $line_counter++;

  $theData = fgets($fh);
  $line_counter++;
  while ($theData!='')
  {
    // Split read line in elements
    $value=split( ';' ,$theData);

    // Insert Family
    if (!oostpoort_insert_member($value))
    {
       // Query failed
       $page.=t('Line ').'['.$line_counter.'] '.t('contains an error (Family insert failed)').'<br/>';
    }
    $line_counter++;
    $theData = fgets($fh);
  }
  fclose($fh);

  // Delete csv file
  unlink($myFile);

  $page .= '<br/>';
  $page .= t('CSV bestand verwerkt!').'<br/>';
  $page .= '<br/>';
  $page .= '<form action="'.url(OOSTPOORT_FAMILY_LIST.'/'.$_SESSION["sort"]).'" method="POST">';
  $page .= '<input type="submit" value="'.t('Terug').'" />';
  $page .= '</form>';

  $page .= '</div>';
  return $page;
}

/**
 * Render a page for the csv addressbook upload
 * @return HTML
 */
function oostpoort_upload_csv() {

  global $user;

  // Breadcrumb menu
  $page  = '<div class="breadcrumb">';
  $page .= '<a href="'.url("").'">Home</a> > ';
  $page .= '<a href="'.url(OOSTPOORT_MENU).'">Menu</a> > ';
  $page .= 'Upload CSV';
  $page .= '</div>';

  $page .= '<div class="addressbook">';

  drupal_set_title(t('Leden administratie csv upload') );

  //only administrators can access this module
  if (!user_access('access administration pages')) {
    return drupal_access_denied();;
  }

   // Fetch Image upload in a HTTP request
  if ($_FILES['uploadedfile']['name']!='') {
    if ( (!strstr($_FILES['uploadedfile']['name'],'.csv')) && (!strstr($_FILES['uploadedfile']['name'],'.CSV')) )
    {
      $page .= '<br/>';
      $page.=t('Only CSV format is supported').'<br/>';
      $page .= '<br/>';

      // Return to family view page
      $page .= '<form action="/" method="POST">';
      $page .= '<input type="submit" value="'.t('Annuleren').'" />';
      $page .= '</form>';

      $page .= '</div>';
      return $page;
    }
    else
    {
      if (move_uploaded_file($_FILES['uploadedfile']['tmp_name'],'files/upload.csv')) {
        watchdog('user', 'A csv file is uploaded to the oostpoort');
        $page .= oostpoort_process_csv_upload();
      }
      else 
      {
        $page .= '<br/>';
        $page .= t('Failed to save csv file, try again!').'<br/>';
        $page .= '<br/>';

        // Return to family view page
        $page .= '<form action="/" method="POST">';
        $page .= '<input type="submit" value="'.t('Annuleren').'" />'.'<br/>';
        $page .= '</form>';

        $page .= '</div>';
        return $page;
      }
    }
  }

  if ($_FILES['uploadedfile']['name']=='') 
  {
    $page .= '<form enctype="multipart/form-data" action="upload" method="POST">';
    $page .= '<br/>';
    $page .= '<input type="hidden" />';
    $page .= t('Choose a csv file to upload:').' <input name="uploadedfile" type="file" />';

    $page .= '<br/>';
    $page .= '<div id="mission">';
    $page .= t('Warning: Only csv formatted files are supported.');
    $page .= '<br/>';
    $page .= t('File size must not exceed').' '.ini_get('upload_max_filesize').'B';
    $page .= '<br/></div>';
    $page .= '<br/>';

    // Show menu Bar
    $page .= '<table>';
    $page .= '<tr><td>';
    $page .= '<input type="hidden" name="fid" value="'.$fid.'" />';
    $page .= '<input type="submit" value="'.t('Bestand uploaden').'" />';
    $page .= '</form>';
    $page .= '</td><td>';

    // Return to family view page
    $page .= '<form action="'.url(OOSTPOORT_MENU).'" method="POST">';
    $page .= '<input type="submit" value="'.t('Terug').'" />';
    $page .= '</form>';

    $page .= '</td></tr>';
    $page .= '</table>';
  }

  $page .= '</div>';
  print theme("page", $page);
}

// ##################################################################################
// THE END
// ##################################################################################