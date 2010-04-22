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


/**
 * Render a visit administration page
 *
 * @return HTML
 */
function oostpoort_visit_admin()  
{
  global $user;

  $block=$_POST['block'];
  if (strlen($block)==0) $block="0";

  $action=$_POST['action'];
  //echo $action;
  
  // Breadcrumb menu
  $page  = '<div class="breadcrumb">';
  $page .= '<a href="'.url("").'">Home</a> > ';
  $page .= '<a href="'.url(OOSTPOORT_MENU).'">Menu</a> > ';
  $page .= 'Family > Block';
  $page .= '</div>';

  if (empty($action)) { 
  
	  $page = oostpoort_visit_admin_show_members();

  } else if ($action=="save") { 
  
     oostpoort_visit_admin_save();
	  $page = oostpoort_visit_admin_show_members();
	  
  } else if ($action=="add") {
  
     $page = "Add screen";
  }
  
  print theme("page", $page);
}

function oostpoort_visit_admin_show_members()
{
  $page .= '<div class="oostpoort">';

  drupal_set_title(t('Oostpoort bezoekwerk administratie' ) );

  $page .= '<br/>';

  $page .= '<form method="POST">';
  
  $query  = 'select ';
  $query .= ' concat(b.Prs_Roepnaam, " ", b.Prs_Tussenvoegsels, " ", b.Prs_Achternaam) as Naam, ';
  $query .= ' b.Prs_GeslachtMV as geslacht, '; 
  $query .= ' b.Prs_id as PrsId, ';
  $query .= ' c.Blk_Nummer as BlkNummer, ';
  $query .= ' c.Blk_Omschrijving as BlkOmschrijving, ';
  $query .= ' a.Mdw_ActiefJN as MdwActief, ';
  $query .= ' d.Rol_Omschrijving as RolOmschrijving ';
  $query .= 'from ';
  $query .= ' oostpoort_medewerker a, oostpoort_persoon b, oostpoort_blok c, oostpoort_rol d ';
  $query .= 'where ';
  $query .= ' a.Mdw_Prs_ID=b.Prs_id and '; 
  $query .= ' a.Mdw_Blk_ID=c.Blk_id and '; 
  $query .= ' a.Mdw_Rol_ID=d.Rol_id ';
  $query .= 'order by '; 
  $query .= ' naam';
  
  $queryResult = db_query($query);
  
  // Show Banner
  $page .= '<table border="1" cellpadding="3" cellspacing="3">';
  $page .= '<tr>';
  $page .= '<td><b>'.t('Actief').'</b></td>';
  $page .= '<td><b>'.t('Id').'</b></td>';
  $page .= '<td><b>'.t('Medewerkernaam').'</b></td>';
  $page .= '<td><b>'.t('Geslacht').'</b></td>';
  $page .= '<td><b>'.t('Rol').'</b></td>';
  $page .= '<td><b>'.t('Blok Nummer').'</b></td>';
  $page .= '<td><b>'.t('Blok Omschrijving').'</b></td>';  
  $page .= '</tr>';

  // Show all found address
  $page_tmp='';
  while ($data = db_fetch_object($queryResult))
  {
     $page_tmp .= '<tr>';
	  $page_tmp .= '<td>'.oostpoort_visit_admin_checkbox("id",$data->PrsId, $data->MdwActief).'</td>';	  
	  $page_tmp .= '<td>'.$data->PrsId.'</td>';
	  $page_tmp .= '<td>'.$data->Naam.'</td>';
     $page_tmp .= '<td>'.oostpoort_visit_admin_geslacht($data->geslacht).'</td>';
	  $page_tmp .= '<td>'.$data->RolOmschrijving.'</td>';
	  $page_tmp .= '<td>'.$data->BlkNummer.'</td>';
	  $page_tmp .= '<td>'.$data->BlkOmschrijving.'</td>';    
	  $page_tmp .= '</tr>';
  }

  if ( $page_tmp!='') 
  {
     $page .= $page_tmp;
  }
  else 
  {
    // No content found
    $page .= '<tr>';
    $page .= '<td>'.t('Geen informatie gevonden').'</td>';
    $page .= '<td></td>';
    $page .= '<td></td>';
    $page .= '<td></td>'; 
	 $page .= '</tr>';
  }
  $page .= '</table>';
  
  // Show button Bar
  $page .= '<br/>';
  $page .= '<table align="left">';
  $page .= '<tr>';
  
  $page .= '<td>';
  $page .= '<input type="submit" name="commit" value="'.t('Opslaan').'" />';
  $page .= '<input type="hidden" name="action" value="save" />';
  $page .= '</form>';
  $page .= '</td>';

  $page .= '<td>';
  $page .= '<form method="POST">';
  $page .= '<input type="submit" name="commit" value="'.t('Toevoegen').'" />';
  $page .= '<input type="hidden" name="action" value="add" />';
  $page .= '</form>';
  $page .= '</td>';
  $page .= '</tr>';
  $page .= '</table>';
  
  $page .= '</div>';
  
  return $page;
}

function oostpoort_visit_admin_save()
{
	$query  = 'select '; 
	$query .= ' a.Mdw_Prs_ID as PrsId, ';
	$query .= ' a.Mdw_ActiefJN as active ';
	$query .= 'from ';
	$query .= ' oostpoort_medewerker a ';
  
	$queryResult = db_query($query);
  
	while ($data = db_fetch_object($queryResult))
	{
		$value=$_POST['id'.$data->PrsId];
	 
		if ($value == 'on') 
		{
			if ($data->active == "ONWAAR")
			{			  
				$sql = 'UPDATE oostpoort_medewerker SET Mdw_ActiefJN="WAAR" where Mdw_Prs_ID='.$data->PrsId;
				//echo $sql;
				db_query($sql);
			}
		} 
		else 
		{     
		   if ($data->active == "WAAR")
			{
				$sql = 'UPDATE oostpoort_medewerker SET Mdw_ActiefJN="ONWAAR" where Mdw_Prs_ID='.$data->PrsId;
				//echo $sql;
				db_query($sql);
		   }
		}
	}  
}
		  
		  
function oostpoort_visit_admin_geslacht($label)
{
   if ( $label=='M') {
		return t('Man');
   } else {
      return t('Vrouw');
   }
}

function oostpoort_visit_admin_checkbox($label, $id, $value)
{
   if ( $value=='WAAR') {
		return '<input type="checkbox" name="'.$label.$id.'" checked="true" />';
   } else {
      return '<input type="checkbox" name="'.$label.$id.'" />';
   }
}


// ##################################################################################
// THE END
// ##################################################################################