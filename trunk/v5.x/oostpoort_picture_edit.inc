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
// EDIT IMAGE
// ##################################################################################

/**
 * Render a page for uploading a picture.
 * @return HTML
 */
function oostpoort_picture_edit( $mid=0 ) 
{
  global $user;

  // Breadcrumb menu
  $page  = '<div class="breadcrumb">';
  $page .= '<a href="'.url("").'">Home</a> > ';
  $page .= '<a href="'.url(OOSTPOORT_MENU).'">Menu</a> > ';
  $page .= '<a href="'.url(OOSTPOORT_FAMILY_LIST).'/'.$_SESSION["sort"].'">Family</a> > ';
  $page .= '<a href="'.url(OOSTPOORT_FAMILY_VIEW.'/'.$parameters).'">View</a> > ';
  $page .= '<a href="'.url(OOSTPOORT_MEMBER_VIEW.'/'.$mid).'">Member</a> > ';
  $page .= 'Add Picture';
  $page .= '</div>';

  $page .= '<div class="oostpoort">';

  drupal_set_title(t('Foto toevoegen') );

  // Fetch Image upload in a HTTP request
  if ($_FILES['uploadedfile']['name']!='') 
  {
    if ( (strstr($_FILES['uploadedfile']['name'],'.jpg')) || (strstr( $_FILES['uploadedfile']['name'],'.JPG')) )
    {
      $error='0';
      if(!is_dir(OOSTPOORT_IMAGE_DIR))
      {
        if( !mkdir(OOSTPOORT_IMAGE_DIR) )
        {
          $page .= '<br/>';
          $page .= t('Creating image directory failed!');
          $page .= '<br/>';
          $error='1';
        }
      }
      $filename= date('YmdHis', time()).'.jpg';

       // Check dimensions of image
      $dimensions = getimagesize($_FILES['uploadedfile']['tmp_name']);
      $width  = $dimensions[0];
      $height = $dimensions[1];
      $resize = 0;

      // Limit picture 
      if ($width>$height)
      {
         if ($width>0) $ratio = 800 / $width;
         if ($ratio < 1)
         {
            $width=800;
            $height=$height*$ratio;
            $resize = 1;
         }
      }
      else 
      {
         if ($width>0) $ratio = 600 / $width;
         if ($ratio < 1)
         {
            $width=600;
            $height=$height*$ratio;
            $resize = 1;
         }
      }

      $page .= '<br/>';

      // Resize uploaded picture
      if ( $resize == 1 )
      {
         if (!image_scale($_FILES['uploadedfile']['tmp_name'], file_create_path(OOSTPOORT_IMAGE_DIR.'/'.$filename), $width, $height)) {
              $page .= '<br>';
              $page .= t('Bestand is te groot! Verklein foto (bv 800x600 pixels) en probeer het opnieuw.');
              $error='1';
              $page .= '<br>';
         }
      }
      else
      {
        // Not resize needed, move file to storage place
        copy($_FILES['uploadedfile']['tmp_name'],OOSTPOORT_IMAGE_DIR.'/'.$filename);
      }

      if ($error==0)
      {
        // Create thumbnails directory store
        if(!is_dir(OOSTPOORT_THUMBNAILS_DIR))
        {
          if( !mkdir(OOSTPOORT_THUMBNAILS_DIR) )
          {
            $page .= '<br/>';
            $page .= t('Creating image directory failed!');
            $page .= '<br/>';
            $error='1';
          }
        }

        $page .= '<br/>';

        $resize = 0;

        // Limit picture size on screen
        if ($width>$height)
        {
         if ($width>0) $ratio  = 160 / $width;
         if ($ratio < 1)
         {
            $width=160;
            $height=$height*$ratio;
            $resize = 1;
         }
        }
        else
        {
         if ($width>0) $ratio  = 120 / $width;
         if ($ratio < 1)
         {
            $width=120;
            $height=$height*$ratio;
            $resize = 1;
         }
        }

        // Create thumbnail picture
        // Resize uploaded picture
        if ( $resize == 1 )
        {
          if (!image_scale(OOSTPOORT_IMAGE_DIR.'/'.$filename, file_create_path(OOSTPOORT_THUMBNAILS_DIR.'/'.$filename), $width, $height)) {
            $page .= '<br>';
            $page .= t('Bestand is te groot! Verklein foto (bijvoorbeeld naar 800x600 pixels) en probeer het opnieuw.');
            $error='1';
            $page .= '<br>';
          }
        }
        else
        {
          // Not resize needed, move file to storage place
          copy(OOSTPOORT_IMAGE_DIR.'/'.$filename, OOSTPOORT_THUMBNAILS_DIR.'/'.$filename);
        }

        if ($error=='0') 
        {
          oostpoort_set_item($mid,8,$filename);
          return oostpoort_member_view($mid);
        }
      }
    }
    else 
    {
      $page .= '<br/>';
      $page.=t('Only JPG image format is supported');
      $page .= '<br/>';
      $_FILES['uploadedfile']['name']='';
    }
  }

  if ($_FILES['uploadedfile']['name']=='') 
  {
    $page .= '<form enctype="multipart/form-data" action="'.url(OOSTPOORT_PICTURE_EDIT.'/').$mid.'" method="POST">';
    $page .= '<br/>';
    $page .= '<input type="hidden" />';
    $page .= t('Selecteer een pasfoto bestand:').' <input name="uploadedfile" type="file" />';

    $page .= '<br/>';
    $page .= '<br/>';
    $page .= '<div id="mission">';
    $page .= t('Waarschuwing: Alleen jpg foto formaat wordt ondersteund.');
    $page .= '<br/>';
    $page .= t('Pasfoto mag niet groter zijn dan ').ini_get('upload_max_filesize').'B';
    $page .= '<br/></div>';
    $page .= '<br/>';

    // Show menu Bar
    $page .= '<table>';
    $page .= '<tr><td>';
    $page .= '<input type="hidden" name="fid" value="'.$fid.'" />';
    $page .= '<input type="submit" value="'.t('Pasfoto uploaden').'" />';
    $page .= '</form>';
    $page .= '</td><td>';

    // Return to member view page
    $page .= '<form action="'.url(OOSTPOORT_MEMBER_VIEW.'/').$mid.'" method="POST">';
    $page .= '<input type="hidden" name="fid" value="'.$fid.'" />';
    $page .= '<input type="submit" value="'.t('Annuleren').'" />';
    $page .= '</form>';

    $page .= '</td></tr>';
    $page .= '</table>';
  }
  else
  {
    $page .= '<br/>';

    // Return to member view page
    $page .= '<form action="'.url(OOSTPOORT_PICTURE_EDIT.'/').$mid.'" method="POST">';
    $page .= '<input type="hidden" name="fid" value="'.$fid.'" />';
    $page .= '<input type="submit" value="'.t('Terug').'" />';
    $page .= '</form>';
  }

  $page .= '</div>';
  print theme("page", $page);
}


// ##################################################################################
// THE END
// ##################################################################################