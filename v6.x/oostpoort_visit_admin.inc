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
  echo "[action=".$action."]";
  
  $id=$_POST["Id"];
  echo "[id=".$id."]";
    
  // Breadcrumb menu
  $page  = '<div class="breadcrumb">';
  $page .= '<a href="'.url("").'">Home</a> > ';
  $page .= '<a href="'.url(OOSTPOORT_MENU).'">Menu</a> > ';
  $page .= 'Family > Block';
  $page .= '</div>';

  if (empty($action)) { 
  
	  $page = oostpoort_visit_admin_show_list();

  } else if ($action=="Save1") { 
  
     oostpoort_visit_admin_save1();
	  $page = oostpoort_visit_admin_show_list();
	  
  } else if ($action=="Edit") {
  
     $page = oostpoort_visit_admin_show_member($id);
	  
  } else if ($action=="Add") {
  
     $page = "Add screen";
	  
  } else if ($action=="Cancel") {
  
     $page = oostpoort_visit_admin_show_list();
	  
  } else if ($action=="Save2") { 
  
	  oostpoort_visit_admin_save2();
     $page = oostpoort_visit_admin_show_list();
  }
  
  print theme("page", $page);
}

function oostpoort_visit_admin_show_list()
{
	$page .= '<div class="oostpoort">';

	drupal_set_title(t('Oostpoort bezoekwerk administratie' ) );

	$page .= '<br/>';

	$page .= '<form name="OostpoortForm" method="POST" >';
  

	
	$query  = 'select ';
	$query .= ' concat(b.Prs_Roepnaam, " ", b.Prs_Tussenvoegsels, " ", b.Prs_Achternaam) as Naam, ';
	$query .= ' b.Prs_GeslachtMV as geslacht, '; 
	$query .= ' b.Prs_id as PrsId, ';
	$query .= ' c.Blk_Nummer as BlkNummer, ';
	$query .= ' c.Blk_Omschrijving as BlkOmschrijving, ';
	$query .= ' a.Mdw_ActiefJN as MdwActief, ';
	$query .= ' d.Rol_Omschrijving as RolOmschrijving, ';
	$query .= ' a.Mdw_UID as MdwUid ';
	$query .= 'from ';
	$query .= ' oostpoort_medewerker a, oostpoort_persoon b, oostpoort_blok c, oostpoort_rol d ';
	$query .= 'where ';
	$query .= ' a.Mdw_Prs_ID=b.Prs_id and '; 
	$query .= ' a.Mdw_Blk_ID=c.Blk_id and '; 
	$query .= ' a.Mdw_Rol_ID=d.Rol_id ';
	$query .= 'order by '; 
	$query .= ' Naam';
  
	$queryResult = db_query($query);
  		
	// Show Banner
	$page .= '<table border="1" cellpadding="3" cellspacing="3">';
	$page .= '<tr>';
	$page .= '<td><b>'.t('Rij').'</b></td>';
	$page .= '<td><b>'.t('Actief').'</b></td>';
	$page .= '<td><b>'.t('Id').'</b></td>';
	$page .= '<td><b>'.t('Medewerkernaam').'</b></td>';
	$page .= '<td><b>'.t('Geslacht').'</b></td>';
	$page .= '<td><b>'.t('Rol').'</b></td>';
	$page .= '<td><b>'.t('Blok').'</b></td>';
	$page .= '<td><b>'.t('Drupal User Id').'</b></td>';   
	$page .= '</tr>';

	// Show all found address
	while ($data = db_fetch_object($queryResult))
	{
		$page .= '<tr>';
		$page .= '<td>'.++$i.'</td>';	  
		$page .= '<td>'.oostpoort_view_checkbox("id",$data->PrsId, $data->MdwActief).'</td>';	  
		$page .= '<td>'.$data->PrsId.'</td>';	  	  
		$page .= '<td>'.oostpoort_hiddenlink('OostpoortForm', 'Id', $data->PrsId, 'Edit', $data->Naam).'</td>';
		$page .= '<td>'.oostpoort_visit_admin_geslacht($data->geslacht).'</td>';
		$page .= '<td>'.$data->RolOmschrijving.'</td>';
		$page .= '<td>Blok '.$data->BlkNummer.'</td>';
		$page .= '<td>'.$data->MdwUid.'</td>';    
		$page .= '</tr>';
	}
	$page .= '</table>';
  
	// Show button Bar
	$page .= '<br/>';
	$page .= '<table align="left">';
	$page .= '<tr>';
	
	$page .= '<td>';
	$page .= oostpoort_hiddenlink('OostpoortForm','Id','0','Save1',t('Opslaan'));
	$page .= '</td>';
	
	$page .= '<td>';
	$page .= oostpoort_hiddenlink('OostpoortForm','Id','0','Add',t('Toevoegen'));
	$page .= '</td>';
	
	$page .= '</tr>';
	$page .= '</table>';
  
	$page .= '</form>';  
	$page .= '</div>';
  
	return $page;
}
	
	
function oostpoort_visit_admin_show_member($id)
{
   global $user;
 
	$page .= '<div class="oostpoort">';

	drupal_set_title(t('Oostpoort bezoekwerk administratie' ) );

	$page .= '<br/>';

	$page .= '<form name="OostpoortForm" method="POST" >';
  
	$query  = 'select ';
	$query .= ' b.Prs_id as PrsId, ';
	$query .= ' concat(b.Prs_Roepnaam, " ", b.Prs_Tussenvoegsels, " ", b.Prs_Achternaam) as Naam, ';
	$query .= ' b.Prs_GeslachtMV as Geslacht, '; 
	$query .= ' a.Mdw_Blk_ID as MdwBlkID, ';
	$query .= ' a.Mdw_ActiefJN as MdwActief, ';
	$query .= ' a.Mdw_Rol_ID as MdwRolID ';
	$query .= 'from ';
	$query .= ' oostpoort_medewerker a, oostpoort_persoon b ';
	$query .= 'where ';
	$query .= ' a.Mdw_Prs_ID=b.Prs_id and '; 
	$query .= ' a.Mdw_Prs_ID = '.$id;
	
	$queryResult = db_query($query);
	$data = db_fetch_object($queryResult);
	
	// Show Banner
	$page .= '<table border="0" cellpadding="2" cellspacing="2">';
	$page .= '<tr>';
	$page .= '<td>'.t('Eigenaar').'</td>';
	$page .= '<td>'.oostpoort_view_owner($user->uid, false).'</td>';
	$page .= '</tr>';
	$page .= '<tr>';
	$page .= '<td>'.t('Id').'</td>';
	$page .= '<td>'.$data->PrsId.'</td>';
	$page .= '</tr>';
	$page .= '<tr>';
	$page .= '<td>'.t('Naam').'</td>';
	$page .= '<td>'.$data->Naam.'</td>';
	$page .= '</tr>';
	$page .= '<tr>';
	$page .= '<td>'.t('Geslacht').'</td>';
	$page .= '<td>'.oostpoort_visit_admin_geslacht($data->Geslacht).'</td>';
	$page .= '</tr>';
	$page .= '<tr>';
	$page .= '<td>'.t('Blok').'</td>';
	$page .= '<td>'.oostpoort_view_block($data->MdwBlkID, false).'</td>';
	$page .= '</tr>';
	$page .= '<tr>';
	$page .= '<td>'.t('Actief').'</td>';
	$page .= '<td>'.oostpoort_view_checkbox("Active", "", $data->MdwActief).'</td>';	  
	$page .= '</tr>';
	$page .= '<tr>';
	$page .= '<td>'.t('Rol Omschrijving').'</td>';
	$page .= '<td>'.oostpoort_view_role($data->MdwRolID, false).'</td>';
	$page .= '</tr>';	
	$page .= '</table>';
    
	// Show button Bar
	$page .= '<br/>';
	$page .= '<table align="left">';
	$page .= '<tr>';
	
	$page .= '<td>';
	$page .= oostpoort_hiddenlink('OostpoortForm','Id',$data->PrsId,'Save2',t('Opslaan'));
	$page .= '</td>';
	
	$page .= '<td>';
	$page .= oostpoort_hiddenlink('OostpoortForm','','','Cancel',t('Terug'));
	$page .= '</td>';
	
	$page .= '</tr>';
	$page .= '</table>';
  
	$page .= '</form>';  
	$page .= '</div>';
	return $page;
}	
	
function oostpoort_hiddenlink($form,$key,$value,$action,$label)
{
   return '<a href="#" onClick="hiddenInput(\''.$form.'\', \''.$key.'\', \''.$value.'\',\''.$action.'\');">'.$label.'</a>';
}

function oostpoort_visit_admin_save1()
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


function oostpoort_visit_admin_save2()
{

   $block=$_POST["Block"];
   echo "[block=".$block."]";
  
   $active=$_POST["Active"];
   echo "[active=".$active."]";
	
	$owner=$_POST["Owner"];
   echo "[owner=".$owner."]";

   $role=$_POST["Role"];
   echo "[role=".$role."]";
	
	$id=$_POST["Id"];
   echo "[id=".$id."]";

	  
	if ($active == 'on') {
	
		$query  = 'UPDATE oostpoort_medewerker SET ';
		$query .= '  Mdw_ActiefJN="WAAR", ';
		$query .= '  Mdw_Blk_ID='.$block.', ';
		$query .= '  Mdw_Uid='.$owner.', ';
		$query .= '  Mdw_Rol_ID='.$role.' ';
		$query .= 'where ';
		$query .= '  Mdw_Prs_ID='.$id;
		
	} else {     
	
		$query  = 'UPDATE oostpoort_medewerker SET ';
		$query .= '  Mdw_ActiefJN="ONWAAR", ';
		$query .= '  Mdw_Blk_Id='.$block.', ';
		$query .= '  Mdw_Uid='.$owner.', ';
		$query .= '  Mdw_Rol_ID='.$role.' ';
		$query .= 'where ';
		$query .= '  Mdw_Prs_ID='.$id;
		
	}
	
	echo $query;
	db_query($query);  
}
		  
		  
function oostpoort_visit_admin_geslacht($label)
{
   if ( $label=='M') {
		return t('Man');
   } else {
      return t('Vrouw');
   }
}

function oostpoort_view_checkbox($label, $id, $value)
{
   if ( $value=='WAAR') {
		return '<input type="checkbox" name="'.$label.$id.'" checked="true" />';
   } else {
      return '<input type="checkbox" name="'.$label.$id.'" />';
   }
}

function oostpoort_view_owner($uid, $readonly) {

   global $user;

   if ( $readonly=='0' ) {
		$query  = 'SELECT uid, name FROM {users} order by name';
		$queryResult = db_query($query);     
		$data = db_fetch_object($queryResult);

		$page.='<select name="Owner">';
		while ($data = db_fetch_object($queryResult)) {
		
			$page.='<option value="'.$data->uid.'" ';
			if ($data->uid==$uid) $page.='selected="selected" ';
			$page.='>'.$data->name.' ['.$data->uid.']</option>';
		}
		$page.='</select>';
	  
   } else {
	 
      $query  = 'SELECT uid, name FROM {users} WHERE uid='.$uid;
      $queryResult = db_query($query);
      $data = db_fetch_object($queryResult);
		 
      $page .= '<input id="text" name="owner" size="20" maxlength="20" type="text" value="'.$data->name.'" READONLY />';
   }
   return $page;
}

function oostpoort_view_block($block, $readonly) {

   if ( $readonly=='0' ) {
		$query  = 'SELECT Blk_ID, Blk_nummer, Blk_Omschrijving FROM {oostpoort_blok} order by Blk_nummer';
		$queryResult = db_query($query);     
		
		$page.='<select name="Block">';
		while ($data = db_fetch_object($queryResult)) {
		
			$page.='<option value="'.$data->Blk_ID.'" ';
			if ($data->Blk_ID==$block) $page.='selected="selected" ';
			$page.='>'.$data->Blk_Omschrijving.' [Blok '.$data->Blk_nummer.']</option>';
		}
		$page.='</select>';
	  
   } else {
	 
      $query  = 'SELECT Blk_ID, Blk_nummer, Blk_Omschrijving FROM {oostpoort_blok} WHERE Blk_ID='.$block;
      $queryResult = db_query($query);
      $data = db_fetch_object($queryResult);
		 
      $page .= $data->Blk_Omschrijving.' [Blok '.$data->Blk_nummer.']';
   }
   return $page;
}


function oostpoort_view_role($role, $readonly) {

   if ( $readonly=='0' ) {
		$query  = 'SELECT Rol_ID, Rol_Omschrijving FROM {oostpoort_rol} order by Rol_Omschrijving';
		$queryResult = db_query($query);     
		
		$page.='<select name="Role">';
		while ($data = db_fetch_object($queryResult)) {
		
			$page.='<option value="'.$data->Rol_ID.'" ';
			if ($data->Rol_ID==$role) $page.='selected="selected" ';
			$page.='>'.$data->Rol_Omschrijving.'</option>';
		}
		$page.='</select>';
	  
   } else {
	 
      $query  = 'SELECT Rol_ID, Rol_Omschrijving FROM {oostpoort_rol} WHERE Rol_ID='.$role;
      $queryResult = db_query($query);
      $data = db_fetch_object($queryResult);
		 
      $page .= $data->Rol_Omschrijving;
   }
   return $page;
}

// ##################################################################################
// THE END
// ##################################################################################