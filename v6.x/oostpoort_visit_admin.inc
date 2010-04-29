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
// STATE MACHINE
// ##################################################################################

/**
 * Render a visit administration page
 *
 * @return HTML
 */
function oostpoort_visit_admin()  
{
	$action=$_POST['action'];
	echo "[action=".$action."]";
        	  
	if ($action=="Edit") {
  
     $page .= oostpoort_visit_admin_member_view();
	  
	} else if ($action=="Search") {
  
     $page .= oostpoort_visit_admin_member_search();
	  
	} else if ($action=="Cancel") {
  
     $page = oostpoort_visit_admin_list_view();
	  
	} else if ($action=="Save") { 
  
	  $page .= oostpoort_visit_admin_save();
     $page .= oostpoort_visit_admin_list_view();
	  
	}  else if ($action=="Add") {
	
     $page .= oostpoort_visit_admin_add();
     $page .= oostpoort_visit_admin_member_view();
	  
	} else if ($action=="Delete") {
  
     $page .= oostpoort_visit_admin_delete();
     $page .= oostpoort_visit_admin_list_view();
	  
	} else if ($action=="MainMenu") {
	
		oostpoort_mainmenu();
		$page="";
	  
	} else {
  
	  $page .= oostpoort_visit_admin_list_view();
	}
  
	if ($page!="") {
		print theme("page", $page);
	}
}

// ##################################################################################
// PAGES
// ##################################################################################

function oostpoort_visit_admin_list_view()
{
	$page .= '<div class="oostpoort">';

	// Create Breadcrumb menu
	$page  = '<div class="breadcrumb">';
	$page .= '<a href="'.url("").'">Start</a> > ';
	$page .= '<a href="'.url(OOSTPOORT_MENU).'">Oostpoort</a> > ';
	$page .= t('Bezoekwerk Admin');
	$page .= '</div>';
    
	drupal_set_title(t('Oostpoort bezoekwerk administratie' ) );

	$page .= '<br/>';
	$page .= t('Dit zijn alle bezoekmedewerkers van de oostpoort.');

	$page .= '<form name="OostpoortForm" method="POST" >';
  	
	$query  = 'select ';	
	$query .= ' concat(IF(b.Prs_Roepnaam="",b.Prs_Voornamen, b.Prs_Roepnaam)," ", b.Prs_Tussenvoegsels, " ", b.Prs_Achternaam) as Naam, ';
	$query .= ' b.Prs_GeslachtMV as geslacht, '; 
	$query .= ' b.Prs_id as PrsId, ';
	$query .= ' c.Blk_Nummer as BlkNummer, ';
	$query .= ' c.Blk_Omschrijving as BlkOmschrijving, ';
	$query .= ' a.Mdw_ActiefJN as MdwActief, ';
	$query .= ' d.Rol_Omschrijving as RolOmschrijving, ';
	$query .= ' a.Mdw_UID as MdwUid, ';
	$query .= ' (select count(*) as Amount from oostpoort_bezoek where Bzk_Mdw_ID=a.Mdw_ID) as Amount ';
	$query .= 'from ';
	$query .= ' oostpoort_medewerker a, oostpoort_persoon b, oostpoort_blok c, oostpoort_rol d ';
	$query .= 'where ';
	$query .= ' a.Mdw_Prs_ID=b.Prs_id and '; 
	$query .= ' a.Mdw_Blk_ID=c.Blk_id and '; 
	$query .= ' a.Mdw_Rol_ID=d.Rol_id ';
	$query .= 'order by '; 
	$query .= ' Naam';
  
	$queryResult = db_query($query);
  			
	$page .= '<table>';
	$page .= '<thead>';
	$page .= '<tr>';
	//$page .= '<th><b>'.t('Rij').'</b></th>';
	$page .= '<th><b>'.t('Id').'</b></th>';
	$page .= '<th><b>'.t('Medewerker').'</b></th>';
	$page .= '<th><b>'.t('Geslacht').'</b></th>';
	$page .= '<th><b>'.t('Rol').'</b></th>';
	$page .= '<th><b>'.t('Blok').'</b></th>';
	$page .= '<th><b>'.t('Login Naam').'</b></th>';   
	$page .= '<th><b>'.t('Aantal bezoeken').'</b></th>';   
	$page .= '</tr>';
	$page .= '</thead>';

	// Show all found members
	$page .= '<tbody>';
	while ($data = db_fetch_object($queryResult))
	{
	   if ((++$i%2)==0) {
			$page .= '<tr class="even">';
		} else {
			$page .= '<tr class="odd">';
		}
		//$page .= '<td>'.$i.'</td>';	  
	   $page .= '<td>'.$data->PrsId.'</td>'; 
		if ($data->MdwActief=="ONWAAR") {
			$page .= '<td>'.oostpoort_hiddenlink('OostpoortForm', 'Id', $data->PrsId, 'Edit', $data->Naam).'</td>';
		} else {
			$page .= '<td><b>'.oostpoort_hiddenlink('OostpoortForm', 'Id', $data->PrsId, 'Edit', $data->Naam).'</b></td>';		
		}
		$page .= '<td>'.oostpoort_visit_geslacht($data->geslacht).'</td>';
		$page .= '<td>'.$data->RolOmschrijving.'</td>';
		$page .= '<td>Blok '.$data->BlkNummer.'</td>';
		$page .= '<td>'.oostpoort_view_owner($data->MdwUid,true).'</td>';    
		$page .= '<td>'.$data->Amount.'</td>';    
		$page .= '</tr>';
	}
	$page .= '</tbody>';
	$page .= '</table>';
    
	// Button bar
 	$page .= oostpoort_hiddenlink('OostpoortForm','Id','0','Search',t('Toevoegen')).' | ';
	$page .= oostpoort_hiddenlink('OostpoortForm','','','MainMenu',t('Terug'));

		
	$page .= '</form>';  
	$page .= '</div>';
  
	return $page;
}
	
	
function oostpoort_visit_admin_member_view()
{
   global $user;
   
	// Get posted parameters		
	$id=$_POST["Id"];
   echo "[id=".$id."]";
    
	// Breadcrumb menu
	$page  = '<div class="breadcrumb">';
	$page .= '<a href="'.url("").'">Start</a> > ';
	$page .= '<a href="'.url(OOSTPOORT_MENU).'">Oostpoort</a> > ';
	$page .= 'Visit > Admin > Member';
	$page .= '</div>';
		
	// Query
	$query  = 'select ';
	$query .= ' b.Prs_id as PrsId, ';
	$query .= ' concat(b.Prs_Voornamen," ",b.Prs_Tussenvoegsels," ",b.Prs_Achternaam) as Naam, ';
	$query .= ' b.Prs_GeslachtMV as Geslacht, '; 
	$query .= ' a.Mdw_Blk_ID as MdwBlkID, ';
	$query .= ' a.Mdw_ActiefJN as MdwActief, ';
	$query .= ' a.Mdw_Rol_ID as MdwRolID, ';
	$query .= ' a.Mdw_UID as MdwUid, ';
	$query .= ' b.Prs_Roepnaam as PrsRoepnaam, ';
	$query .= ' b.Prs_EmailAdres as PrsEmail, ';
	$query .= ' b.Prs_TelefoonMobiel as PrsMobiel, ';
	$query .= ' concat(c.Adr_Straat, " " ,c.Adr_Huisnr) as straat, ';
	$query .= ' c.Adr_Plaats as AdrPlaats, ';
	$query .= ' c.Adr_Postcode as AdrPostcode, ';	
	$query .= ' c.Adr_Telefoon as AdrTelefoon, ';	
	$query .= ' (select count(*) as Amount from oostpoort_bezoek where Bzk_Mdw_ID=a.Mdw_ID) as Amount ';
	$query .= 'from ';
	$query .= ' oostpoort_medewerker a, oostpoort_persoon b, oostpoort_adres c ';
	$query .= 'where ';
	$query .= ' a.Mdw_Prs_ID=b.Prs_id and '; 
	$query .= ' c.Adr_ID=b.Prs_Huidig_Adr_ID and '; 
	$query .= ' a.Mdw_Prs_ID = '.$id;
	
	$queryResult = db_query($query);
	$data = db_fetch_object($queryResult);
	
	drupal_set_title(t('Oostpoort bezoekwerk administratie' ) );
		
	$page .= '<form name="OostpoortForm" method="POST" >';
	$page .= '<div class="oostpoort">';
	
	$page .= '<fieldset>' ;
	$page .= '<legend>Medewerker</legend>';
	$page .= '<h4>'.$data->Naam.'</h4>';	
	$page .= '<h4>'.$data->straat.'</h4>';	
	$page .= '<h4>'.$data->AdrPlaats.' '.$data->AdrPostcode.'</h4>';		
	$page .= '</fieldset>' ;
	
	$page .= '<fieldset>' ;
	$page .= '<legend>Instellingen</legend>';
		
	$page .= '<div class="form-item">';
	$page .= '<label>'.t('Eigenaar').': <span class="form-required">*</span></label>';
	$page .= oostpoort_view_owner($data->MdwUid, false);
	$page .= '<div class="description">Selecteer hier de login naam van de medewerker.</div>';
	$page .= '</div>';

	$page .= '<div class="form-item">';
	$page .= '<label>'.t('Roepnaam').': <span class="form-required">*</span></label>';
	$page .= '<input id="text" name="FirstName" size="20" maxlength="20" type="text" value="'.$data->PrsRoepnaam.'">';
	$page .= '<div class="description">Roepnaam van medewerker.</div>';
	$page .= '</div>';

	$page .= '<div class="form-item">';
	$page .= '<label>'.t('Telefoon').':</label>';
	$page .= '<input id="text" name="Phone" size="20" maxlength="20" type="text" value="'.$data->AdrTelefoon.'">';
	$page .= '<div class="description">Telefoon nummer van medewerker.</div>';
	$page .= '</div>';
	
	$page .= '<div class="form-item">';
	$page .= '<label>'.t('Mobiel').':</label>';
	$page .= '<input id="text" name="Mobile" size="20" maxlength="20" type="text" value="'.$data->PrsMobiel.'">';
	$page .= '<div class="description">Mobiele nummer van medewerker.</div>';
	$page .= '</div>';

	$page .= '<div class="form-item">';
	$page .= '<label>'.t('Email').': <span class="form-required">*</span></label>';
	$page .= '<input id="text" name="Email" size="60" maxlength="60" type="text" value="'.$data->PrsEmail.'">';
	$page .= '<div class="description">Email adres van medewerker.</div>';
	$page .= '</div>';

	$page .= '<div class="form-item">';
	$page .= '<label>'.t('Blok').': <span class="form-required">*</span></label>';
	$page .= oostpoort_view_block($data->MdwBlkID, false, true);
	$page .= '<div class="description">Selecteer blok waar medewerker actief is.</div>';
	$page .= '</div>';

	$page .= '<div class="form-item">';
	$page .= '<label>'.t('Actief').':</label>';
	$page .= oostpoort_view_checkbox($data->MdwActief, false);	  
	$page .= '<div class="description">Actief of niet actief.</div>';
	$page .= '</div>';
	
	$page .= '<div class="form-item">';
	$page .= '<label>'.t('Rol Omschrijving').': <span class="form-required">*</span></label>';
	$page .= oostpoort_view_role($data->MdwRolID, false);
	$page .= '<div class="description">Selecteer welk rol deze medewerker heeft.</div>';
	$page .= '</div>';
    	 
	$page .= oostpoort_hiddenlink('OostpoortForm','Id',$data->PrsId,'Save',t('Opslaan'));
		
	// Only when no visit work is executed visit member can be deleted
	if ($data->Amount==0) {
		$page .= ' | ';
		$page .= oostpoort_hiddenlink('OostpoortForm','Id',$data->PrsId,'Delete',t('Verwijderen'));
	}
	
	$page .= ' | ';	
	$page .= oostpoort_hiddenlink('OostpoortForm','','','Cancel',t('Terug'));
  
   $page .= '</fieldset>' ;  
	$page .= '</form>';  
	$page .= '</div>';
	return $page;
}	

function oostpoort_visit_admin_member_search()
{
   // Get posted parameters
   $search=$_POST['search'];
   echo "[search=".$search."]";
 
	$page .= '<div class="oostpoort">';

	// Page title
	drupal_set_title(t('Oostpoort bezoekwerk administratie' ) );

		
	// Breadcrumb menu
	$page  = '<div class="breadcrumb">';
	$page .= '<a href="'.url("").'">Start</a> > ';
	$page .= '<a href="'.url(OOSTPOORT_MENU).'">Oostpoort</a> > ';
	$page .= 'Visit > Admin > Member > Search';
	$page .= '</div>';
	
	$page .= '<br/>';
  
   $page .= '<form name="OostpoortForm" method="POST" >';  
	
   $page .= '<table>';	

   $page .= '<tr>';
   $page .= '<td>';
	$page .= t('Voer een achternaam in:');
   $page .= '</td>';
	$page .= '</tr>';
	
   $page .= '<tr>';
   $page .= '<td>';

   $page .= '<input id="text" name="search" size="65" maxlength="65" type="text" value="'.$search.'"> ';
	$page .= oostpoort_hiddenlink('OostpoortForm','',0,'Search',t('Zoeken'));
   $page .= '</td>';	
	$page .= '</tr>';
	
	$page .= '</table>';
   
	if ($search!="")
	{
		$query  = 'select ';
		$query .= ' concat(IF(b.Prs_Roepnaam="",b.Prs_Voornamen, b.Prs_Roepnaam)," ", b.Prs_Tussenvoegsels, " ", b.Prs_Achternaam) as Naam, ';
		$query .= ' b.Prs_Id as PrsId, ';
		$query .= ' b.Prs_GeslachtMV as PrsGeslacht, ';
		$query .= ' b.Prs_Geboortedatum as PrsGeboortedatum ';
		$query .= 'from ';
		$query .= ' oostpoort_persoon b ';
		$query .= 'where ';
		$query .= ' b.Prs_Achternaam LIKE "'.$search.'%"  '; 
		$query .= 'order by ';
		$query .= ' Naam';
		//echo $query;
		
		$queryResult = db_query($query);
  		
		// Show Banner
		$page .= t('Als u een persoon selecteerd wordt hij automatisch toegevoegd aan de bezoekmedewerkerslijst');
		
		$page .= '<table>';
		$page .= '<thead>';
		$page .= '<tr>';
		$page .= '<th><b>'.t('Rij').'</b></th>';
		$page .= '<th><b>'.t('Naam').'</b></th>';
		$page .= '<th><b>'.t('Id').'</b></th>';
		$page .= '<th><b>'.t('Geslacht').'</b></th>';  
		$page .= '<th><b>'.t('Geboortedatm').'</b></th>';
		$page .= '</tr>';
		$page .= '</thead>';
		$page .= '<tbody>';

		// Show all found address
		while ($data = db_fetch_object($queryResult))
		{
			if ((++$i%2)==0) {
				$page .= '<tr class="even">';
			} else {
				$page .= '<tr class="odd">';
			}
			$page .= '<td>'.$i.'</td>';	  
			$page .= '<td>'.oostpoort_hiddenlink('OostpoortForm', 'Id', $data->PrsId, 'Add', $data->Naam).'</td>';
			$page .= '<td>'.$data->PrsId.'</td>';	  	  
			$page .= '<td>'.oostpoort_admin_geslacht($data->PrsGeslacht).'</td>'; 
			$page .= '<td>'.oostpoort_view_date($data->PrsGeboortedatum).'</td>'; 
			$page .= '</tr>';
		}
		$page .= '</tbody>';
		$page .= '</table>';
	}
	$page .= oostpoort_hiddenlink('OostpoortForm','','','Cancel',t('Terug'));
	
	$page .= '</form>';  
	$page .= '</div>';
	
	return $page;
}
		
// ##################################################################################
// ACTIONS
// ##################################################################################
 
function oostpoort_visit_admin_save() {
	
	// Get Posted parameters
	$id=$_POST["Id"];
   echo "[id=".$id."]";
	
	$page .= '<div class="messages status">';
   $page .= t('Wijzingen voor medewerker '.$id.' zijn opgeslagen.');
	$page .= '</div>';
	
	$block=$_POST["Block"];
   echo "[block=".$block."]";
  
   $active=$_POST["Active"];
   echo "[active=".$active."]";
	
	$owner=$_POST["Owner"];
   echo "[owner=".$owner."]";

   $role=$_POST["Role"];
   echo "[role=".$role."]";
	
	$firstName=$_POST["FirstName"];
   echo "[firstName=".$firstName."]";

	$email=$_POST["Email"];
   echo "[email=".$email."]";

	$phone=$_POST["Phone"];
   echo "[phone=".$phone."]";
	
	$mobile=$_POST["Mobile"];
   echo "[mobile=".$mobile."]";	
	  
	if ($active == 'on') { 
	    $active_token="WAAR";
	} else {
		$active_token="ONWAAR";
	}
	
	$query  = 'UPDATE oostpoort_medewerker SET ';
	$query .= '  Mdw_ActiefJN="'.$active_token.'", ';
	$query .= '  Mdw_Blk_ID='.$block.', ';
	$query .= '  Mdw_Uid='.$owner.', ';
	$query .= '  Mdw_Rol_ID='.$role.' ';
	$query .= 'where ';
	$query .= '  Mdw_Prs_ID='.$id;

	//echo $query;
	db_query($query); 
			
	$query  = 'UPDATE oostpoort_persoon SET ';
	$query .= '  Prs_Roepnaam="'.$firstName.'", ';
	$query .= '  Prs_EmailAdres="'.$email.'", ';
	$query .= '  Prs_TelefoonMobiel="'.$mobile.'" ';
	$query .= 'where ';
	$query .= '  Prs_ID='.$id;
	
	echo $query;
	db_query($query); 
	
	$query  = 'select ';
	$query .= '  Prs_Huidig_Adr_ID as AdrId ';
	$query .= 'from ';
	$query .= '  oostpoort_persoon ';
	$query .= 'where ';
	$query .= '  Prs_ID='.$id;
	
	echo $query;
	$queryResult = db_query($query);
	$data = db_fetch_object($queryResult);
				
	$query  = 'UPDATE oostpoort_adres SET ';
	$query .= '  Adr_Telefoon="'.$phone.'" ';
	$query .= 'where ';
	$query .= '  Adr_ID='.$data->AdrId;
	
	echo $query;
	db_query($query);  
	
	return $page;	
}
		  
function oostpoort_visit_admin_add()
{
	$page .= '<div class="messages status">';
   $page .= t('Medewerker '.$id.' is toegevoegd.');
	$page .= '</div>';
	
	// Get posted parameters		
	$id=$_POST["Id"];
   echo "[id=".$id."]";
				
	$query  = 'select ';
	$query .= ' a.Mdw_Prs_ID as MdwPrsID ';
	$query .= 'from ';
	$query .= ' oostpoort_medewerker a ';
	$query .= 'where ' ;
	$query .= ' a.Mdw_Prs_ID='.$id  ;
	
	// Check if user already exist as visit member
	$queryResult = db_query($query);     
	$data = db_fetch_object($queryResult);
	//echo $query;
	
	if ($data->MdwPrsID=="")
	{
	   // Does not exist, create entry
		$query  = 'INSERT INTO oostpoort_medewerker (';
		$query .= '  `Mdw_ID`, `Mdw_Prs_ID`, `Mdw_Rol_ID`, `Mdw_ActiefJN`, `Mdw_Blk_ID`, `Mdw_UID`)';
		$query .= 'VALUES ';
		$query .= '( '.$id.','.$id.', 2, "WAAR", 10, 0 )';
	
		//echo $query;
		db_query($query); 
	}
	
	return $page;
}
 
function oostpoort_visit_admin_delete() {

	$page .= '<div class="messages status">';
   $page .= t('Medewerker '.$id.' is verwijderd.');
	$page .= '</div>';

   $id=$_POST["Id"];
   echo "[id=".$id."]";
	
	// Delete selected visit member
	$query  = 'delete from ';
	$query .= ' oostpoort_medewerker ';
	$query .= 'where ';
	$query .= ' Mdw_Prs_ID='.$id  ;	
	
	//echo $query;
	db_query($query); 
	
	return $page;
}

// ##################################################################################
// UTILS
// ##################################################################################

function oostpoort_hiddenlink($form,$key,$value,$action,$label) {
   return '<a href="#" onClick="hiddenInput(\''.$form.'\', \''.$key.'\', \''.$value.'\',\''.$action.'\');">'.$label.'</a>';
}
 
function oostpoort_visit_geslacht($label) {
   if ( $label=='M') {
		return t('Man');
   } else {
      return t('Vrouw');
   }
}

function oostpoort_view_checkbox( $value, $readonly) {

   if (! $readonly) {
	
		if ( $value=='WAAR') {
			return '<input type="checkbox" name="Active" checked="true" />';
		} else {
			return '<input type="checkbox" name="Active" />';
		}
	} else {
		return $value;
	}
}

function oostpoort_view_owner($uid, $readonly) {

   global $user;

   if ( ! $readonly ) {
		$query  = 'SELECT uid, name FROM {users} order by name';
		$queryResult = db_query($query);     
		$data = db_fetch_object($queryResult);

		$page.='<select name="Owner">';

		$page.='<option value="0"';
			if ($uid==0) $page.='selected="selected" ';
		$page.='>&nbsp;</option>';
					
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
		 
      $page .= $data->name;
   }
   return $page;
}

function oostpoort_view_block($block, $readonly, $omschrijving) {

   if ( ! $readonly ) {
		$query  = 'SELECT Blk_ID, Blk_nummer, Blk_Omschrijving FROM {oostpoort_blok} order by Blk_nummer';
		$queryResult = db_query($query);     
		
		$page.='<select name="Block">';
		while ($data = db_fetch_object($queryResult)) {
		
			$page.='<option value="'.$data->Blk_ID.'" ';
			if ($data->Blk_ID==$block) $page.='selected="selected" ';
			if ($omschrijving) {
				$page.='>'.$data->Blk_Omschrijving.' [Blok '.$data->Blk_nummer.']</option>';
			} else {
				$page.='>Blok '.$data->Blk_nummer.'</option>';
			}
		}
		$page.='</select> ';
	  
   } else {
	 
      $query  = 'SELECT Blk_ID, Blk_nummer, Blk_Omschrijving FROM {oostpoort_blok} WHERE Blk_ID='.$block;
      $queryResult = db_query($query);
      $data = db_fetch_object($queryResult);
		 
      $page .= $data->Blk_Omschrijving.' [Blok '.$data->Blk_nummer.']';
   }
   return $page;
}


function oostpoort_view_role($role, $readonly) {

   if ( ! $readonly ) {
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


function oostpoort_view_date($date) { 

  $page = "";
  
  if ($date!=0) {
		list($year, $month, $day) = split('[/.-]', $date);
		$page = $day.'-'.$month.'-'.$year;
  }
  return $page;
}


// ##################################################################################
// THE END
// ##################################################################################