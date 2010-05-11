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
	
	// Check access for this page.
	if (!oostpoort_check_admin_access()) {
		return drupal_access_denied();
	}
		
	$Action=$_POST['Action'];
	if ($Action=="") {
		$Action=$_SESSION['Action'];
	}
	oostpoort_debug_param("Action",$Action);	
	$_SESSION['Action']=$Action;
		  	  
	if ($Action=="Edit") {
  
     $page .= oostpoort_visit_admin_member();
	  
	} else if ($Action=="Search") {
  
     $page .= oostpoort_visit_admin_search();
	  
	} else if ($Action=="Cancel") {
  
     $page = oostpoort_visit_admin_list();
	  
	} else if ($Action=="Save") { 
  
	  oostpoort_visit_admin_save();
     $page .= oostpoort_visit_admin_list();
	  
	}  else if ($Action=="Add") {
	
     oostpoort_visit_admin_add();
     $page .= oostpoort_visit_admin_member();
	  
	} else if ($Action=="Delete") {
  
     oostpoort_visit_admin_delete();
     $page .= oostpoort_visit_admin_list();
	  
	} else {
  
	  $page .= oostpoort_visit_admin_list();
	}
  
	print theme("page", $page);
}

// ##################################################################################
// VIEWS
// ##################################################################################

function oostpoort_visit_admin_list() {
 	// Remove access rights for this module it will be updated if you enter the main menu again.
	unset($_SESSION['MdwId']);

	// Build page
	$page .= '<form name="OostpoortForm" method="POST" >';
	$page .= '<div class="oostpoort">';

	// Breadcrumb menu
	$node["child1"] = t('Bezoekwerk Administratie');
	oostpoort_breadcrumb($node);

	// title
	drupal_set_title(t('Bezoekwerk administratie' ) );
		
	$page .= '<br/>';
	$page .= t('Dit zijn alle bezoekmedewerkers van de oostpoort.');

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
  			
	$page .= '<table>';
	$page .= '<thead>';
	$page .= '<tr>';
	$page .= '<th><b>'.t('Foto').'</b></th>';
	$page .= '<th><b>'.t('Actief').'</b></th>';
	$page .= '<th><b>'.t('Medewerker').'</b></th>';
	$page .= '<th><b>'.t('Admin').'</b></th>';
	$page .= '<th><b>'.t('Rol').'</b></th>';
	$page .= '<th><b>'.t('Blok').'</b></th>';
	$page .= '<th><b>'.t('Login').'</b></th>';   
	$page .= '<th><b>'.t('Opmerkingen').'</b></th>';   
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
		$page .= '<td>';
		$page .= oostpoort_load_picture("Prs",$data->PrsId, 30, 25, "Left", false);
		$page .= '</td>';

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
		$page .= 'Geplande: '.$data->GeplandTotal.'<br/>';
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
 	$page .= oostpoort_hiddenlink('OostpoortForm','PrsId','0','Search',t('Toevoegen')).' | ';
	$page .= '<a href="'.url(OOSTPOORT_MENU).'">'.t('Terug').'</a>';

	$page .= '</div>';
	$page .= '</form>';  

	return $page;
}
	
function oostpoort_visit_admin_member()
{
   global $user;
   
	// Get posted parameters		
	$PrsId=$_POST["PrsId"];
   oostpoort_debug_param("PrsId",$PrsId);
   
	// Build page
	$page .= '<form name="OostpoortForm" method="POST" >';
	$page .= '<div class="oostpoort">';
	
	// Breadcrumb menu
	$node["child1"] = t('Bezoekwerk Administratie');
	$node["child2"] = t('Bezoekmedewerker Details');
	oostpoort_breadcrumb($node);
	
	// title
	drupal_set_title(t('Bezoekwerkmedewerker Details' ) );
			
	// Query
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
		
	$page .= '<fieldset>' ;
	$page .= '<legend>Medewerker</legend>';
	$page .= oostpoort_load_picture("Prs",$data->PrsId,0,0,"right", true);
	$page .= '<h4>'.$data->Naam.'</h4>';	
	$page .= '<h4>'.$data->straat.'</h4>';	
	$page .= '<h4>'.$data->AdrPostcode.'&nbsp;&nbsp;'.$data->AdrPlaats.'</h4>';	

	$page .= '<div class="form-item">';
	$page .= '<label>'.t('Medewerker Id').':</label>';
	$page .= $data->PrsId;
	$page .= '</div>';
		
	$page .= '<div class="form-item">';
	$page .= '<label>'.t('Kerkelijke lid nummer').':</label>';
	$page .= $data->PrsKerklidnummer;
	$page .= '</div>';
	
	$page .= '<div class="form-item">';
	$page .= '<label>'.t('Geslacht').':</label>';
	$page .= oostpoort_view_geslacht($data->PrsGeslacht);
	$page .= '</div>';
	
	$page .= '<div class="form-item">';
	$page .= '<label>'.t('Geboorte Datum').':</label>';
	$page .= oostpoort_convert_date($data->PrsGeboorteDatum);
	$page .= '</div>';

	$page .= '<div class="form-item">';
	$page .= '<label>'.t('Huwelijks Datum').':</label>';
	$page .= oostpoort_convert_date($data->PrsHuwelijksDatum);
	$page .= '</div>';
	
	$page .= '</fieldset>' ;
	
	$page .= '<fieldset>' ;
	$page .= '<legend>Instellingen</legend>';
		
	$page .= '<div class="form-item">';
	$page .= '<label>'.t('Eigenaar').': <span class="form-required">*</span></label>';
	$page .= oostpoort_view_owner($data->MdwUid, false);
	$page .= '<div class="description">';
	$page .= t('Selecteer hier de login naam van de medewerker. Zo lang een bezoek medewerker niet gekoppeld is aan een login account kan hij geen bezoeken in plannen.');
	$page .= '</div>';
	$page .= '</div>';

	$page .= '<div class="form-item">';
	$page .= '<label>'.t('Roepnaam').': <span class="form-required">*</span></label>';
	$page .= '<input id="text" name="FirstName" size="30" maxlength="30" type="text" value="'.$data->PrsRoepnaam.'">';
	$page .= '<div class="description">';
	$page .= t('Roepnaam van medewerker.');
	$page .= '</div>';
	$page .= '</div>';

	$page .= '<div class="form-item">';
	$page .= '<label>'.t('Telefoon').':</label>';
	$page .= '<input id="text" name="Phone" size="15" maxlength="15" type="text" value="'.$data->AdrTelefoon.'">';
	$page .= '<div class="description">';
	$page .= t('Telefoon nummer van medewerker.');
	$page .= '</div>';
	$page .= '</div>';
	
	$page .= '<div class="form-item">';
	$page .= '<label>'.t('Mobiel').':</label>';
	$page .= '<input id="text" name="Mobile" size="15" maxlength="15" type="text" value="'.$data->PrsMobiel.'">';
	$page .= '<div class="description">';
	$page .= t('Mobiele nummer van medewerker.');
	$page .= '</div>';
	$page .= '</div>';

	$page .= '<div class="form-item">';
	$page .= '<label>'.t('Email').': <span class="form-required">*</span></label>';
	$page .= '<input id="text" name="Email" size="60" maxlength="100" type="text" value="'.$data->PrsEmail.'">';
	$page .= '<div class="description">';
	$page .= t('Email adres van medewerker.');
	$page .= '</div>';
	$page .= '</div>';

	$page .= '<div class="form-item">';
	$page .= '<label>'.t('Blok').': <span class="form-required">*</span></label>';
	$page .= oostpoort_view_block($data->MdwBlkID, false, true);
	$page .= '<div class="description">';
	$page .= t('Selecteer blok waar medewerker actief is.');
	$page .= '</div>';
	$page .= '</div>';

	$page .= '<div class="form-item">';
	$page .= '<label>'.t('Actief').':</label>';
	$page .= oostpoort_view_checkbox("Active", $data->MdwActief, false);	  
	$page .= '<div class="description">';
	$page .= t('Actief of niet actief.');
	$page .= '</div>';
	$page .= '</div>';
	
	$page .= '<div class="form-item">';
	$page .= '<label>'.t('Admin').':</label>';
	$page .= oostpoort_view_checkbox("Admin", $data->MdwAdmin, false);	  
	$page .= '<div class="description">';
	$page .= t('Heeft medewerker (Admin rechten) zodat hij medewerkers kan beheren.');
	$page .= '</div>';
	$page .= '</div>';
	
	$page .= '<div class="form-item">';
	$page .= '<label>'.t('Rol Omschrijving').': <span class="form-required">*</span></label>';
	$page .= oostpoort_view_role($data->MdwRolID, false);
	$page .= '<div class="description">';
	$page .= t('Selecteer welk rol deze medewerker heeft.');
	$page .= '</div>';
	$page .= '</div>';
    	 
	$page .= oostpoort_hiddenlink('OostpoortForm','PrsId',$data->PrsId,'Save',t('Opslaan'));
		
	// Only when no visit work is executed visit member can be deleted
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

function oostpoort_visit_admin_search()
{
   // Get posted parameters
   $Search=$_POST['Search'];
   oostpoort_debug_param("Search",$Search);

	// Build page
   $page .= '<form name="OostpoortForm" method="POST" >';  
	$page .= '<div class="oostpoort">';
 		
	// Breadcrumb menu
	$node["child1"] = t('Bezoekwerk Administratie');
	$node["child2"] = t('Bezoekmedewerker toevoegen');
	oostpoort_breadcrumb($node);
	
	// Page title
	drupal_set_title( t('Bezoekmedewerker toevoegen') );
	
	if ($Search=="") {
		$page .= '<div class="messages error">';
		$page .= t('Geen achternaam ingevoerd!');
		$page .= '</div>';	
	}
	
	$page .= '<fieldset>' ;
	$page .= '<legend>'.t('Zoeken').'</legend>';
	$page .= '<div class="form-item">';
	$page .= '<label>'.t('Achternaam').' :</label>';
	$page .= '<input id="text" name="Search" size="65" maxlength="65" type="text" value="'.$Search.'"> ';
	$page .= oostpoort_hiddenlink('OostpoortForm','','','Search',t('Zoek'));
	$page .= '<div class="description">';
	$page .= t('Voer een achternaam in.');
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
		//$page .= '<th><b>'.t('Rij').'</b></th>';
		$page .= '<th>'.t('Foto').'</b></th>';
		$page .= '<th class="active"><b>'.t('Naam').'</b></th>';
		//$page .= '<th><b>'.t('PrsId').'</b></th>';
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
			//$page .= '<td>'.$i.'</td>';	  
			$page .= '<td>'.oostpoort_load_picture("Prs",$data->PrsId, 30, 25, "Left", false).'</td>';
			$page .= '<td class="active">'.oostpoort_hiddenlink('OostpoortForm', 'PrsId', $data->PrsId, 'Add', $data->Naam).'</td>';
			//$page .= '<td>'.$data->PrsId.'</td>';	  	  
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
	
	// Get Posted parameters
	$PrsId=$_POST["PrsId"];
	oostpoort_debug_param("PrsId",$PrsId);
		
	$block=$_POST["Block"];
	oostpoort_debug_param("Block",$block);
  
   	$active=$_POST["Active"];
	oostpoort_debug_param("Active",$active);
	
	$admin=$_POST["Admin"];
	oostpoort_debug_param("Admin",$admin);
	
	$owner=$_POST["Owner"];
	oostpoort_debug_param("Owner",$owner);

   	$role=$_POST["Role"];
	oostpoort_debug_param("Role",$role);
 	
	$firstName=$_POST["FirstName"];
	oostpoort_debug_param("FirstName",$firstname);
  
	$email=$_POST["Email"];
	oostpoort_debug_param("Email",$email);
 
	$phone=$_POST["Phone"];
	oostpoort_debug_param("Phone",$phone);
	
	$mobile=$_POST["Mobile"];
   	oostpoort_debug_param("Mobile",$mobile);
	  
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

	$msg .= t('Wijzingen voor medewerker '.$PrsId.' zijn opgeslagen.');
	drupal_set_message($msg, 'status');	
	
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
			
	$query  = 'UPDATE oostpoort_persoon SET ';
	$query .= '  Prs_Roepnaam="'.$firstName.'", ';
	$query .= '  Prs_EmailAdres="'.$email.'", ';
	$query .= '  Prs_TelefoonMobiel="'.$mobile.'" ';
	$query .= 'where ';
	$query .= '  Prs_ID='.$PrsId;
	oostpoort_debug_sql($query);
	db_query($query); 
	
	$query  = 'select ';
	$query .= '  Prs_Huidig_Adr_ID as AdrId ';
	$query .= 'from ';
	$query .= '  oostpoort_persoon ';
	$query .= 'where ';
	$query .= '  Prs_ID='.$PrsId;
	oostpoort_debug_sql($query);
	$queryResult = db_query($query);
	$data = db_fetch_object($queryResult);
							
	$query  = 'UPDATE oostpoort_adres SET ';
	$query .= '  Adr_Telefoon="'.$phone.'" ';
	$query .= 'where ';
	$query .= '  Adr_ID='.$data->AdrId;
	oostpoort_debug_sql($query);
	db_query($query);  	
}
		  
function oostpoort_visit_admin_add() {
	
	// Get posted parameters		
	$PrsId=$_POST["PrsId"];
	oostpoort_debug_param("PrsId",$PrsId);
	
	$msg .= t('Medewerker '.$PrsId.' is toegevoegd.');
	drupal_set_message($msg, 'status');	
					
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
	
	if ($data->MdwPrsID=="")
	{
	   // Does not exist, create entry
		$query  = 'INSERT INTO oostpoort_medewerker (';
		$query .= '  `Mdw_ID`, `Mdw_Prs_ID`, `Mdw_Rol_ID`, `Mdw_ActiefJN`, `Mdw_Blk_ID`, `Mdw_UID`)';
		$query .= 'VALUES ';
		$query .= '( '.$PrsId.','.$PrsId.', 2, "WAAR", 10, 0 )';
		oostpoort_debug_sql($query);
		db_query($query); 
	}
}
 
function oostpoort_visit_admin_delete() {

   $PrsId=$_POST["PrsId"];
	oostpoort_debug_param("PrsId",$PrsId);
	
	$msg .= t('Medewerker '.$PrsId.' is verwijderd.');
	drupal_set_message($msg, 'status');	
	
	// Delete selected visit member
	$query  = 'delete from ';
	$query .= ' oostpoort_medewerker ';
	$query .= 'where ';
	$query .= ' Mdw_Prs_ID='.$PrsId  ;	
	oostpoort_debug_sql($query);
	db_query($query); 
}

// ##################################################################################
// THE END
// ##################################################################################