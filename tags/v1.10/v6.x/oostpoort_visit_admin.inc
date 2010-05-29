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
	//echo var_dump($_POST);
	
	// *****************************
	// Check access for this page.
	// *****************************
	
	if (!oostpoort_visit_member_admin_access()) {
		return drupal_access_denied();
	}
	
	// *****************************
	// Process POST variables
	// *****************************
	
	$Action=htmlentities($_POST['Action']);
	oostpoort_debug_param("Action",$Action);	
		  
	// *****************************
	// Process statemachine
	// *****************************
	  
	switch ($Action) {
	
		case 'Edit':  
			$page = oostpoort_visit_admin_member();
			break;
		
	   case 'Search':  
			$page = oostpoort_visit_admin_search();
			break;
			
		case 'Cancel':
			$page = oostpoort_visit_admin_list();
			break;
		
		case 'Save':
			$error=oostpoort_visit_admin_save();		
			if ($error==0) {
				$page = oostpoort_visit_admin_list();
			} else {
				$page = oostpoort_visit_admin_member();			
			}	
			break;
		
		case 'Add':	
			oostpoort_visit_admin_add();
			$page = oostpoort_visit_admin_member();
			break;
		
		case 'Delete': 
			oostpoort_visit_admin_delete();
			$page = oostpoort_visit_admin_list();
			break;
	  
	   default:
	  		$page = oostpoort_visit_admin_list();
			break;
	}
  
	print theme("page", $page);
}

// ##################################################################################
// VIEWS
// ##################################################################################

function oostpoort_visit_admin_list() {
 	
	// Remove access rights for this module it will be 
	// updated if you enter the main menu again.
	unset($_SESSION['MdwId']);

	// *****************************
	// Query data
	// *****************************
	
	$query  = 'select ';	
	$query .= ' concat(IF(b.Prs_Roepnaam="",b.Prs_Voorletters, b.Prs_Roepnaam)," ", b.Prs_Tussenvoegsels, " ", b.Prs_Achternaam) as Naam, ';
	$query .= ' b.Prs_GeslachtMV as geslacht, '; 
	$query .= ' b.Prs_id as PrsId, ';
	$query .= ' c.Blk_Id as BlkId, ';
	$query .= ' c.Blk_Omschrijving as BlkOmschrijving, ';
	$query .= ' a.Mdw_ActiefJN as MdwActief, ';
	$query .= ' a.Mdw_Admin as MdwAdmin, ';
	$query .= ' d.Rol_Omschrijving as RolOmschrijving, ';
	$query .= ' a.Mdw_UID as MdwUid, ';
	$query .= ' (select count(*) from oostpoort_bezoek where Bzk_Mdw_ID=a.Mdw_ID) as Amount, ';
	$query .= ' (select count(*) from oostpoort_bezoek where Bzk_Mdw_ID=a.Mdw_ID and Bzk_GeplandDatum!=0 and Bzk_AfgelegdDatum=0 and Bzk_GeannuleerdDatum=0 and Bzk_BezoekNietGewenstDatum=0) as GeplandTotal, ';
	$query .= ' (select count(*) from oostpoort_bezoek where Bzk_Mdw_ID=a.Mdw_ID and Bzk_AfgelegdDatum!=0 and  Bzk_BezoekNietGewenstDatum=0 and Bzk_BezoekNietGewenstDatum=0) as AfgelegdTotal, ';
	$query .= ' (select count(*) from oostpoort_bezoek where Bzk_Mdw_ID=a.Mdw_ID and Bzk_GeannuleerdDatum!=0 and Bzk_AfgelegdDatum=0 and Bzk_BezoekNietGewenstDatum=0) as GeannuleerdTotal, ';
	$query .= ' (select count(*) from oostpoort_bezoek where Bzk_Mdw_ID=a.Mdw_ID and Bzk_BezoekNietGewenstDatum!=0) as BezoekNietGewenstTotal ';
	$query .= 'from ';	
	$query .= ' oostpoort_medewerker a, oostpoort_persoon b, oostpoort_blok c, oostpoort_rol d ';
	$query .= 'where ';
	$query .= ' a.Mdw_Prs_ID=b.Prs_id and '; 
	$query .= ' a.Mdw_Blk_ID=c.Blk_id and '; 
	$query .= ' a.Mdw_Rol_ID=d.Rol_id ';
	$query .= 'order by '; 
	$query .= ' b.Prs_Achternaam';
	
	oostpoort_debug_sql($query);
	$queryResult = db_query($query);

	// *****************************
	// Build page
	// *****************************
	
	$page .= '<form name="OostpoortForm" method="POST" >';
	$page .= '<div class="oostpoort">';

	// Breadcrumb menu
	$node["child1"] = l(t('Bezoekwerk Administratie'),OOSTPOORT_VISIT_ADMIN);
	oostpoort_breadcrumb($node);

	// title
	drupal_set_title(t('Bezoekwerk administratie' ) );
		
	$page .= '<br/>';
	$page .= t('Dit zijn alle bezoekmedewerkers van de oostpoort.');
	
	$page .= '<table>';
	$page .= '<thead>';
	$page .= '<tr>';
	if ( variable_get('oostpoort_show_member_picture',1)==1 ) {
		$page .= '<th><b>'.t('Foto').'</b></th>';
	}
	$page .= '<th><b>'.t('Actief').'</b></th>';
	$page .= '<th><b>'.t('Medewerker').'</b></th>';
	$page .= '<th><b>'.t('Admin').'</b></th>';
	$page .= '<th><b>'.t('Rol').'</b></th>';
	$page .= '<th><b>'.t('Blok').'</b></th>';
	$page .= '<th><b>'.t('Login').'</b></th>';   
	$page .= '<th><b>'.t('Overige').'</b></th>';   
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
		
		if ( variable_get('oostpoort_show_member_picture',1)==1 ) {
		
			$page .= '<td>';
			$page .= oostpoort_load_picture("Prs",$data->PrsId, 30, 25, "Left", false);
			$page .= '</td>';
		}

		$page .= '<td>';
		$page .= oostpoort_view_checkbox("Active", $data->MdwActief, true);
		$page .= '</td>';
		
		$page .= '<td>';
		$page .= oostpoort_hiddenlink('OostpoortForm', 'PrsId', $data->PrsId, 'Edit', $data->Naam);
		$page .= '</td>';
		
		$page .= '<td>';
		$page .= oostpoort_view_checkbox("Admin", $data->MdwAdmin, true);
		$page .= '</td>';
	
		$page .= '<td>';
		$page .= $data->RolOmschrijving;
		$page .= '</td>';
		
		$page .= '<td>';
		$page .= $data->BlkId;
		$page .= '</td>';
		
		$page .= '<td>';
		$page .= oostpoort_view_owner($data->MdwUid,true);    
		$page .= '</td>';
		
		$page .= '<td>';
		$page .= '<div class="description">';
		$page .= 'Ingeplande: '.$data->GeplandTotal.'<br/>';
		$page .= 'Afgelegd: '.$data->AfgelegdTotal.'<br/>';
		$page .= 'Geannuleerd: '.$data->GeannuleerdTotal.'<br/>';
		$page .= 'Niet gewenst: '.$data->BezoekNietGewenstTotal.'<br/>';
		$page .= 'Totaal: '.$data->Amount.'<br/>';
		$page .= '</div class="description">';
		$page .= '</td>';
		
		$page .= '</tr>';
	}
	$page .= '</tbody>';
	$page .= '</table>';
    
	// Button bar
 	$page .= oostpoort_hiddenlink('OostpoortForm','PrsId','0','Search',t('Bezoekmedewerker toevoegen')).' | ';
	$page .= '<a href="'.url(OOSTPOORT_MENU).'">'.t('Terug').'</a>';

	$page .= '</div>';
	$page .= '</form>';  

	return $page;
}
	
function oostpoort_visit_admin_member()
{
   global $user;
   
	// *****************************
	// Get posted Inputs
	// *****************************
	
	$PrsId=htmlentities($_POST["PrsId"]);
   oostpoort_debug_param("PrsId",$PrsId);
   
	// *****************************
	// Query data
	// *****************************
	
	$query  = 'select ';
	$query .= ' b.Prs_id as PrsId, ';
	$query .= ' concat(IF(b.Prs_Roepnaam="",b.Prs_Voorletters, b.Prs_Roepnaam)," ",b.Prs_Tussenvoegsels," ",b.Prs_Achternaam) as Naam, ';
	$query .= ' b.Prs_GeslachtMV as Geslacht, '; 
	$query .= ' a.Mdw_Blk_ID as MdwBlkID, ';
	$query .= ' a.Mdw_ActiefJN as MdwActief, ';
	$query .= ' a.Mdw_Admin as MdwAdmin, ';
	$query .= ' a.Mdw_Rol_ID as MdwRolID, ';
	$query .= ' a.Mdw_UID as MdwUid, ';
	$query .= ' b.Prs_Kerklidnummer as PrsKerklidnummer, ';
	$query .= ' b.Prs_Roepnaam as PrsRoepnaam, ';
	$query .= ' b.Prs_EmailAdres as PrsEmail, ';
	$query .= ' b.Prs_TelefoonMobiel as PrsMobiel, ';
	$query .= ' b.Prs_Geboortedatum as PrsGeboorteDatum, ';
	$query .= ' b.Prs_Huwelijksdatum as PrsHuwelijksDatum, ';
	$query .= ' b.Prs_GeslachtMV as PrsGeslacht, '; 
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
	$query .= ' a.Mdw_Prs_ID = '.$PrsId;
	oostpoort_debug_sql($query);
	$queryResult = db_query($query);
	$data = db_fetch_object($queryResult);
	
	// *****************************
	// Get posted Inputs
	// *****************************
	
	if ( isset($_POST["FirstName"]) ) {
		$firstName=htmlentities($_POST["FirstName"]);
	} else {
	   $firstName=$data->PrsRoepnaam;
	}
	oostpoort_debug_param("FirstName",$firstName);
  
	if ( isset($_POST["Email"]) ) {
		$email=htmlentities($_POST["Email"]);
	} else {
	   $email=$data->PrsEmail;
	}
	oostpoort_debug_param("Email",$email);
 
	if ( isset($_POST["Phone"]) ) {
		$phone=htmlentities($_POST["Phone"]);
	} else {
	   $phone=$data->AdrTelefoon;
	}
	oostpoort_debug_param("Phone",$phone);
	
	if ( isset($_POST["Mobile"]) ) {
		$mobile=htmlentities($_POST["Mobile"]);
	} else {
	   $mobile=$data->PrsMobiel;
	}
	oostpoort_debug_param("mobile",$mobile);
	
	if ( isset($_POST["Block"]) ) {
		$block=htmlentities($_POST["Block"]);		
		
		// Checkbox POST tag is only available if active.
		if ( isset($_POST["Active"]) ) {
			$active=htmlentities($_POST["Active"]);
		} else {
			$active="";
		}
		oostpoort_debug_param("Active",$active);

		// Checkbox POST tag is only available if active.
		if ( isset($_POST["Admin"]) ) {
			$admin=htmlentities($_POST["Admin"]);
		} else {
			$admin="";
		}
		oostpoort_debug_param("Admin",$admin);		
			
	} else {
	
	   $block=$data->MdwBlkID;
		
		$active=$data->MdwActief;
		oostpoort_debug_param("Active",$active);
	
		$admin=$data->MdwAdmin;
		oostpoort_debug_param("Admin",$admin);
	}
	oostpoort_debug_param("Block",$block);
		
	if ( isset($_POST["Owner"]) ) {
		$owner=htmlentities($_POST["Owner"]);
	} else {
	   $owner=$data->MdwUid;
	}
	oostpoort_debug_param("Owner",$owner);

	if ( isset($_POST["Role"]) ) {
		$role=htmlentities($_POST["Role"]);
	} else {
	   $role=$data->MdwRolID;
	}
	oostpoort_debug_param("Role",$role);
	
	// *****************************
	// Build page
	// *****************************

	$page .= '<form name="OostpoortForm" method="POST" >';
	$page .= '<div class="oostpoort">';
	
	// Breadcrumb menu
	$node["child1"] = l(t('Bezoekwerk Administratie'),OOSTPOORT_VISIT_ADMIN);
	$node["child2"] = t('Bezoekmedewerker');
	oostpoort_breadcrumb($node);
	
	// title
	drupal_set_title(t('Bezoekmedewerker'));
			
	$page .= '<fieldset>' ;
	$page .= '<legend>Medewerker</legend>';
	
	if ( variable_get('oostpoort_show_member_picture',1)==1 ) {
		$page .= oostpoort_load_picture("Prs",$data->PrsId,0,0,"right", true);
	}
	$page .= '<h4>'.$data->Naam.'</h4>';	
	$page .= '<h4>'.$data->straat.'</h4>';	
	$page .= '<h4>'.$data->AdrPostcode.'&nbsp;&nbsp;'.$data->AdrPlaats.'</h4>';	

	$page .= oostpoort_form_item(
					t('Medewerker Id:'),							
					$data->PrsId,
					'',
					true);

	$page .= oostpoort_form_item(
					t('Kerkelijke lid nummer:'),							
					$data->PrsKerklidnummer,
					'',
					true);
		
	$page .= oostpoort_form_item(
					t('Geslacht:'),							
					oostpoort_view_geslacht($data->PrsGeslacht),
					'',
					true);
	
	$page .= oostpoort_form_item(
					t('Geboorte Datum:'),							
					oostpoort_convert_date($data->PrsGeboorteDatum),
					'',
					true);
					
	$page .= '</fieldset>' ;
	
	$page .= '<fieldset>' ;
	
	$page .= '<legend>'.t('Instellingen').'</legend>';
		
	$page .= oostpoort_form_item(
					t('Eigenaar:'),							
					oostpoort_view_owner($owner, false),
					t('Selecteer hier de login naam van de bezoekmedewerker. Zo lang een bezoekmedewerker niet gekoppeld is aan een drupal login account kan hij geen bezoeken in plannen.'),
					false);
					
	$page .= oostpoort_form_item(
					t('Roepnaam:').oostpoort_view_manitory(),							
					oostpoort_view_text("FirstName", 30, 30, $firstName, false),
					t('Roepnaam van bezoekmedewerker.'),
					false);
					
	$page .= oostpoort_form_item(
					t('Telefoon:').oostpoort_view_manitory(),							
					oostpoort_view_text("Phone", 15, 15, $phone, false),
					t('Telefoon nummer van bezoekmedewerker.'),
					false);
					
	$page .= oostpoort_form_item(
					t('Mobiel:').oostpoort_view_manitory(),							
					oostpoort_view_text("Mobile", 15, 15, $mobile, false),
					t('Mobiele nummer van bezoekmedewerker.'),
					false);

	$page .= oostpoort_form_item(
					t('Email:').oostpoort_view_manitory(),							
					oostpoort_view_text("Email", 60, 100, $email, false),
					t('Email adres van bezoekmedewerker.'),
					false);
					
	$page .= oostpoort_form_item(
					t('Blok:').oostpoort_view_manitory(),							
					oostpoort_view_block($block, false, true),
					t('Selecteer blok waar bezoekmedewerker actief is.'),
					false);
					
	$page .= oostpoort_form_item(
					t('Actief:'),							
					oostpoort_view_checkbox("Active", $active, false),
					t('Actief of niet actief bezoekmedewerker.'),
					false);
					
	$page .= oostpoort_form_item(
					t('Admin:'),							
					oostpoort_view_checkbox("Admin", $admin, false),
					t('Heeft bezoekmedewerker (Admin rechten) zodat hij bezoekmedewerkers kan beheren.'),
					false);
	
	$page .= oostpoort_form_item(
					t('Rol Omschrijving:'),							
					oostpoort_view_role($role, false),
					t('Selecteer welk rol deze medewerker heeft.'),
					false);
					
	$page .= oostpoort_hiddenlink('OostpoortForm','PrsId',$data->PrsId,'Save',t('Opslaan'));
		
	// Only members without any visit can be deleted
	if ($data->Amount==0) {
		$page .= ' | ';
		$page .= oostpoort_hiddenlink('OostpoortForm','PrsId',$data->PrsId,'Delete',t('Verwijderen'));
	}
	
	$page .= ' | ';	
	$page .= oostpoort_hiddenlink('OostpoortForm','','','Cancel',t('Terug'));
  
   $page .= '</fieldset>' ;  
	
	$page .= '</div>';
	$page .= '</form>';  

	return $page;
}	

function oostpoort_visit_admin_search() {
	
	// *****************************
	// Get posted Inputs
	// *****************************
	
	$Search=htmlentities($_POST['Search']);
	oostpoort_debug_param("Search",$Search);

   // *****************************
	// Create page
	// *****************************
	
	$page .= '<form name="OostpoortForm" method="POST" >';  
	$page .= '<div class="oostpoort">';
 		
	// Breadcrumb menu
	$node["child1"] = l(t('Bezoekwerk Administratie'),OOSTPOORT_VISIT_ADMIN);
	$node["child2"] = t('Bezoekmedewerker toevoegen');
	oostpoort_breadcrumb($node);
	
	// Page title
	drupal_set_title( t('Bezoekmedewerker toevoegen') );
	
	if (isset($_POST['Search']) && ($Search=="")) {
	
		$msg = t('Geen achternaam ingevoerd!');
		drupal_set_message( $msg, 'error');		
	}
	
	$page .= '<fieldset>' ;
	$page .= '<legend>'.t('Zoeken').'</legend>';
	$page .= '<div class="form-item">';
	$page .= '<label>'.t('Achternaam').' :</label>';
	$page .= '<input id="text" name="Search" size="65" maxlength="65" type="text" value="'.$Search.'"> ';
	$page .= oostpoort_hiddenlink('OostpoortForm','','','Search',t('Zoek'));
	$page .= '<div class="description">';
	$page .= t('Voer een achternaam van de bezoekmedewerker in.');
	$page .= '</div>';
	$page .= '</div>';
	$page .= '</fieldset>' ;
	
	if ($Search!="") {
		
		$query  = 'select ';
		$query .= ' concat(IF(b.Prs_Roepnaam="",b.Prs_Voorletters, b.Prs_Roepnaam)," ", b.Prs_Tussenvoegsels, " ", b.Prs_Achternaam) as Naam, ';
		$query .= ' b.Prs_Id as PrsId, ';
		$query .= ' b.Prs_GeslachtMV as PrsGeslacht, ';
		$query .= ' b.Prs_Geboortedatum as PrsGeboortedatum ';
		$query .= 'from ';
		$query .= ' oostpoort_persoon b ';
		$query .= 'where ';
		$query .= ' b.Prs_Achternaam LIKE "'.$Search.'%"  '; 
		$query .= 'order by ';
		$query .= ' b.Prs_Achternaam';
		
		oostpoort_debug_sql($query);
		$queryResult = db_query($query);
  		
		// Show Banner
		$page .= t('Als u een persoon selecteerd wordt hij automatisch toegevoegd aan de bezoekmedewerkerslijst');
		
		$page .= '<table>';
		$page .= '<thead>';
		$page .= '<tr>';
		if ( variable_get('oostpoort_show_member_picture',1)==1 ) {
			$page .= '<th>'.t('Foto').'</b></th>';
		}
		$page .= '<th class="active"><b>'.t('Naam').'</b></th>';
		$page .= '<th><b>'.t('Geslacht').'</b></th>';  
		$page .= '<th><b>'.t('Geboren').'</b></th>';
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
  
			if ( variable_get('oostpoort_show_member_picture',1)==1 ) {
				$page .= '<td>'.oostpoort_load_picture("Prs",$data->PrsId, 30, 25, "Left", false).'</td>';
			}
			$page .= '<td class="active">'.oostpoort_hiddenlink('OostpoortForm', 'PrsId', $data->PrsId, 'Add', $data->Naam).'</td>';	  
			$page .= '<td>'.oostpoort_view_geslacht($data->PrsGeslacht).'</td>'; 
			$page .= '<td>'.oostpoort_convert_date($data->PrsGeboortedatum).'</td>'; 
			$page .= '</tr>';
		}
		$page .= '</tbody>';
		$page .= '</table>';
	} 
	$page .= oostpoort_hiddenlink('OostpoortForm','','','Cancel',t('Terug'));
	
	$page .= '</div>';
	$page .= '</form>';  
	
	return $page;
}
		
// ##################################################################################
// ACTIONS
// ##################################################################################
 
function oostpoort_visit_admin_save() {
	
	// *****************************
	// Get posted Inputs
	// *****************************
	
	$PrsId=htmlentities($_POST["PrsId"]);
	oostpoort_debug_param("PrsId",$PrsId);
		
	$block=htmlentities($_POST["Block"]);
	oostpoort_debug_param("Block",$block);
  
	$active=htmlentities($_POST["Active"]);
	oostpoort_debug_param("Active",$active);
	
	$admin=htmlentities($_POST["Admin"]);
	oostpoort_debug_param("Admin",$admin);
	
	$owner=htmlentities($_POST["Owner"]);
	oostpoort_debug_param("Owner",$owner);

	$role=htmlentities($_POST["Role"]);
	oostpoort_debug_param("Role",$role);
 	
	$firstName=htmlentities($_POST["FirstName"]);
	oostpoort_debug_param("FirstName",$firstname);
  
	$email=htmlentities($_POST["Email"]);
	oostpoort_debug_param("Email",$email);
 
	$phone=htmlentities($_POST["Phone"]);
	oostpoort_debug_param("Phone",$phone);
	
	$mobile=htmlentities($_POST["Mobile"]);
   oostpoort_debug_param("Mobile",$mobile);
	  
	// *****************************
	// Input Validation
	// *****************************
	
	$error=0;
	
	if ($firstName=="") {

		$msg = t('Roepnaam is verplicht');
		drupal_set_message( $msg, 'error');
		$error=1;
	}

	if ($phone=="") {

		$msg = t('Telefoonnummer is verplicht');
		drupal_set_message( $msg, 'error');
		$error=1;
	}
	
	if ($email=="") {

		$msg = t('Email is verplicht');
		drupal_set_message( $msg, 'error');
		$error=1;
		
	} else if (oostpoort_validate_email($email)) {

		$msg = t('Email formaat is niet correct');
		drupal_set_message( $msg, 'error');
		$error=1;
	}
	
	if ($error==1) {
	
		// If error return direct!
		return 1;
	} 
	
	// *****************************
	// Database actions
	// *****************************
		
	if ($active == 'on') { 
	    $active_token="WAAR";
	} else {
		$active_token="ONWAAR";
	}
	
	if ($admin == 'on') { 
	    $admin_token="WAAR";
	} else {
		$admin_token="ONWAAR";
	}
	
	// Update user visit data
	$query  = 'UPDATE oostpoort_medewerker SET ';
	$query .= '  Mdw_ActiefJN="'.$active_token.'", ';
	$query .= '  Mdw_Admin="'.$admin_token.'", ';
	$query .= '  Mdw_Blk_ID='.$block.', ';
	$query .= '  Mdw_Uid='.$owner.', ';
	$query .= '  Mdw_Rol_ID='.$role.' ';
	$query .= 'where ';
	$query .= '  Mdw_Prs_ID='.$PrsId;
	oostpoort_debug_sql($query);
	db_query($query); 
			
	// Update user data
	$query  = 'UPDATE oostpoort_persoon SET ';
	$query .= '  Prs_Roepnaam="'.$firstName.'", ';
	$query .= '  Prs_EmailAdres="'.$email.'", ';
	$query .= '  Prs_TelefoonMobiel="'.$mobile.'" ';
	$query .= 'where ';
	$query .= '  Prs_ID='.$PrsId;
	oostpoort_debug_sql($query);
	db_query($query); 
	
	// Find adrId of user
	$query  = 'select ';
	$query .= '  Prs_Huidig_Adr_ID as AdrId ';
	$query .= 'from ';
	$query .= '  oostpoort_persoon ';
	$query .= 'where ';
	$query .= '  Prs_ID='.$PrsId;
	oostpoort_debug_sql($query);
	$queryResult = db_query($query);
	$data = db_fetch_object($queryResult);
						
	// Update Address info.
	$query  = 'UPDATE oostpoort_adres SET ';
	$query .= '  Adr_Telefoon="'.$phone.'" ';
	$query .= 'where ';
	$query .= '  Adr_ID='.$data->AdrId;
	oostpoort_debug_sql($query);
	db_query($query);  	
	
	// *****************************
	// Create message
	// *****************************
	
	$msg .= t('Wijzingen voor medewerker '.$PrsId.' zijn opgeslagen.');
	drupal_set_message($msg, 'status');	
	watchdog('user', $msg);
	
	return 0;
}
		  
function oostpoort_visit_admin_add() {
	
	// *****************************
	// Get posted Inputs
	// *****************************
	
	$PrsId=htmlentities($_POST["PrsId"]);
	oostpoort_debug_param("PrsId",$PrsId);

	// *****************************
	// Database actions
	// *****************************
	
	// Check if user allready exist. Protect against reload button
	$query  = 'select ';
	$query .= ' a.Mdw_Prs_ID as MdwPrsID ';
	$query .= 'from ';
	$query .= ' oostpoort_medewerker a ';
	$query .= 'where ' ;
	$query .= ' a.Mdw_Prs_ID='.$PrsId ;
	
	oostpoort_debug_sql($query);
	$queryResult = db_query($query);     
	$data = db_fetch_object($queryResult);
	
	if ($data->MdwPrsID=="") {
	
	    // User does not exist, create entry
		$query  = 'INSERT INTO oostpoort_medewerker (';
		$query .= '  `Mdw_ID`, `Mdw_Prs_ID`, `Mdw_Rol_ID`, `Mdw_ActiefJN`, `Mdw_Blk_ID`, `Mdw_UID`)';
		$query .= 'VALUES ';
		$query .= '( '.$PrsId.','.$PrsId.', 2, "WAAR", 10, 0 )';
		
		oostpoort_debug_sql($query);
		db_query($query);

		// *****************************
		// Create message
		// *****************************
	
		$msg .= t('Medewerker '.$PrsId.' is toegevoegd.');
		drupal_set_message($msg, 'status');	
		watchdog('user', $msg);
		
		return 0;
		
	} else {
	
		return 1;	
	}
}
 
function oostpoort_visit_admin_delete() {

	// *****************************
	// Get posted Inputs
	// *****************************
	
	$PrsId=htmlentities($_POST["PrsId"]);
	oostpoort_debug_param("PrsId",$PrsId);
		
	// *****************************
	// Database actions
	// *****************************
	
	// Check if user exist. Protect against reload button
	$query  = 'select ';
	$query .= ' a.Mdw_Prs_ID as MdwPrsID ';
	$query .= 'from ';
	$query .= ' oostpoort_medewerker a ';
	$query .= 'where ' ;
	$query .= ' a.Mdw_Prs_ID='.$PrsId ;
	
	oostpoort_debug_sql($query);
	$queryResult = db_query($query);     
	$data = db_fetch_object($queryResult);
	
	if ($data->MdwPrsID!="") {
	
		// Delete selected user
		$query  = 'delete from ';
		$query .= ' oostpoort_medewerker ';
		$query .= 'where ';
		$query .= ' Mdw_Prs_ID='.$PrsId  ;	
		oostpoort_debug_sql($query);
		db_query($query);

		// *****************************
		// Create message
		// *****************************
	
		$msg .= t('Bezoekmedewerker '.$PrsId.' is verwijderd.');
		drupal_set_message($msg, 'status');			
		watchdog('user', $msg);
	
		return 0;
		
	} else  {
		
		return 1;
	}
}

// ##################################################################################
// THE END
// ##################################################################################