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
// VIEW IMAGE
// ##################################################################################

/**
 * Render a page showing the picture full screen.
 * @return HTML
 */
function oostpoort_picture_view( $mid=0 ) 
{
  global $user;

  // Breadcrumb menu
  $page  = '<div class="breadcrumb">';
  $page .= '<a href="'.url("").'">Home</a> > ';
  $page .= '<a href="'.url(OOSTPOORT_MENU).'">Menu</a> > ';
  $page .= '<a href="'.url(OOSTPOORT_FAMILY_LIST).'/'.$_SESSION["sort"].'">Family</a> > ';
  $page .= '<a href="'.url(OOSTPOORT_FAMILY_VIEW.'/'.$parameters).'">View</a> > ';
  $page .= '<a href="'.url(OOSTPOORT_MEMBER_VIEW.'/'.$mid).'">Member</a> > ';
  $page .= 'View Picture';
  $page .= '</div>';

  $page .= '<div class="oostpoort">';

  drupal_set_title(t('Oostpoort foto bekijken') );

  $image = oostpoort_get_item($mid,8);

  $picture_delete = isset($_POST['picture_delete']) ? '1' : '0';
  $go_delete = isset($_POST['go_delete']) ? '1' : '0';

  // Delete selected picture from disk and database
  if (($picture_delete=='1') && ($go_delete=='1')) 
  {
     if (is_file(OOSTPOORT_IMAGE_DIR.'/'.$image))
     {
       unlink(OOSTPOORT_IMAGE_DIR.'/'.$image);
     }
     if (is_file(OOSTPOORT_THUMBNAILS_DIR.'/'.$image)) 
     {
       unlink(OOSTPOORT_THUMBNAILS_DIR.'/'.$image);
     }
     oostpoort_delete_item($mid,8);
     return oostpoort_member_view($mid);
  }

  // Delete are you sure form
  if (($picture_delete=='1') && ($go_delete=='0')) 
  {
    $page .= '<br/>';
    $page .= t('Weet u zeker dat u de pasfoto wilt verwijderen?');
    $page .= '<br/>';
    $page .= '<br/>';
    $page .= '<table align="left">';
    $page .= '<tr>';
    $page .= '<td>';
    $page .= '<form action="'.url(OOSTPOORT_PICTURE_VIEW.'/'.$mid).'" method="POST">';
    $page .= '<input type="submit" name="go_delete" value="'.t('Ja').'" />';
    $page .= '<input type="hidden" name="picture_delete" />';
    $page .= '</form>';
    $page .= '</td>';
    $page .= '<td>';
    $page .= '<form action="'.url(OOSTPOORT_MEMBER_VIEW.'/'.$mid).'" method="POST">';
    $page .= '<input type="submit" name="cancel_delete" value="'.t('Nee').'" />';
    $page .= '</form>';
    $page .= '</td></tr>';
    $page .= '</table>';
    $page .= '</div>';
    return $page;
  }
  else
  {
    // Show menu Bar
    $page .= '<br/>';
    $page .= '<table align="left">';
    $page .= '<tr>';

    $page .= '<td valign="top">';
    $page .= '<form action="'.url(OOSTPOORT_PICTURE_VIEW.'/'.$mid).'" method="POST">';
    $page .= '<input type="submit" name="picture_delete" value="'.t('Delete').'" />';
    $page .= '</form>';
    $page .= '</td>';

    $page .= '<td>';
    $page .= '<form action="'.url(OOSTPOORT_MEMBER_VIEW.'/'.$mid).'" method="POST">';
    $page .= '<input type="submit" name="cancel" value="'.t('Terug').'" />';
    $page .= '</form>';
    $page .= '</td>';

    $page .= '</tr>';
    $page .= '</table>';

    $page .= '<br/>';

    $filename=url(OOSTPOORT_IMAGE_DIR.'/'.$image);
    $page.='<img src="'.$filename.'"/>';
    $page .= '<br/>';
  }

  $page .= '</div>';
  print theme("page", $page);
}


// ##################################################################################
// THE END
// ##################################################################################

