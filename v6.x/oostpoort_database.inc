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
// SQL FUNCTIONS
// ##################################################################################

function oostpoort_get_id($mid)
{
  // Fetch member from database
  $query  = 'SELECT kerklidnr FROM {oostpoort_members} where mid='.$mid;
  $queryResult = db_query($query);
  $data = db_fetch_object($queryResult);
  
  return $data->kerklidnr;
}

function oostpoort_get_item($mid,$type)
{
  // Get unique church member id
  $kerklidnr=oostpoort_get_id($mid);

  // Fetch configuration item
  $query= 'select value from {oostpoort_items} where kerklidnr='.$kerklidnr.' and type='.$type;
  $queryResult = db_query($query);
  $data = db_fetch_object($queryResult);
  
  return $data->value;
}

function oostpoort_get_item2($kerklidnr,$type)
{
  // Fetch configuration item
  $query= 'select value from {oostpoort_items} where kerklidnr='.$kerklidnr.' and type='.$type;
  $queryResult = db_query($query);
  $data = db_fetch_object($queryResult);
  
  return $data->value;
}

function oostpoort_delete_item($mid,$type)
{
  // Get unique church member id
  $kerklidnr=oostpoort_get_id($mid);

  // First delete old entry (if any)
  $query= 'delete from {oostpoort_items} where kerklidnr='.$kerklidnr.' and type='.$type;
  $result = db_query($query);
}

function oostpoort_set_item($mid,$type,$value)
{
  // Get unique church member id
  $kerklidnr=oostpoort_get_id($mid);

  // First delete old entry (if any)
  $query= 'delete from {oostpoort_items} where kerklidnr='.$kerklidnr.' and type='.$type;
  $result = db_query($query);

  if (strlen($value)>0)
  {  
    $query = 'insert into {oostpoort_items} ( kerklidnr, type, value) values ( '.$kerklidnr.','.$type.',"'.$value.'")';
    $result = db_query($query);

    if ($result==1) 
    {
      // Query succesfull
      return true;
    }
    else 
    {
      return false;
    }
  }
  return true;
}

/*
 * insert member SQL function
 * @return true of false
 */
function oostpoort_insert_member( $data ) 
{
  $count=0;
  $query  = 'INSERT INTO oostpoort_members ( ';
  $query .= 'STATUS, WIJKNAAM, SECTIE, GESLACHT, AANSCHRIJF_NAAM, VOEGSEL, VOORNAMEN, STRAATNAAM, HUISNUMMER, ';
  $query .= 'POSTCODE, WOONPLAATS, LEEFTIJD, GEBOORTE_DATUM, GEBOORTE_PLAATS, BURGELIJKESTAAT, KERKELIJKESTAAT, HUWELIJK_DATUM, HUWELIJK_PLAATS, ';
  $query .= 'DOOP_DATUM, DOOP_PLAATS, KERKELIJKE_GEZINDTE, BELIJDENIS_DATUM, BELIJDENIS_PLAATS, HERKOMST_DATUM, HERKOMST_PLAATS, MUTATIE_DATUM, ';
  $query .= 'KERKELIJK_RELATIE, VOORKEUR_WIJK, KERKBLAD, KERKLIDNR, ACHTERNAAM, LAST_UPDATED)';
  $query .= 'VALUES (';

  $status              = '"'.$data[$count++].'"';
  $voorkeur_wijknaam   = '"'.$data[$count++].'"';
  $voorkeur_sectie     = '"'.$data[$count++].'"';
  $wijknaam            = '"'.$data[$count++].'"';
  $geslacht            = '"'.$data[$count++].'"';
  $aanschrijfnaam      = $data[$count++];
  $voegsel             = '"'.$data[$count++].'"';
  $voornamen           = '"'.$data[$count++].'"';
  $straatnaam          = '"'.$data[$count++].'"';

  $huisnummer          = '"'.$data[$count++];
  $huisext             = trim($data[$count++]);
  if (strlen($huisext)>0) $huisnummer.="-".$huisext;
  $huisletter          = trim($data[$count++]);
  if (strlen($huisletter)>0) $huisnummer.=$huisletter;
  $huisnummer         .= '"';

  $postcode            = '"'.$data[$count++].'"';
  $woonplaats          = '"'.$data[$count++].'"';
  $leeftijd            = '"'.$data[$count++].'"';
  $geboorte_datum      = '"'.oostpoort_date($data[$count++]).'"';
  $geboorte_plaats     = '"'.$data[$count++].'"';
  $burgelijkestaat     = '"'.$data[$count++].'"';
  $kerkelijkestaat     = '"'.$data[$count++].'"';
  $huwelijk_datum      = '"'.oostpoort_date($data[$count++]).'"';
  $huwelijk_plaats     = '"'.$data[$count++].'"';
  $doop_datum          = '"'.oostpoort_date($data[$count++]).'"';
  $doop_plaats         = '"'.$data[$count++].'"';
  $kerkelijk_gezindte  = '"'.$data[$count++].'"';
  $belijdenis_datum    = '"'.oostpoort_date($data[$count++]).'"';
  $belijdenis_plaats   = '"'.$data[$count++].'"';
  $herkomst_datum      = '"'.oostpoort_date($data[$count++]).'"';
  $herkomst_plaats     = '"'.$data[$count++].'"';
  $mutatie_datum       = '"'.oostpoort_date($data[$count++]).'"';
  $kerkelijk_relatie   = '"'.$data[$count++].'"';
  $kerkblad            = '"'.$data[$count++].'"';
  $lidnr               = trim($data[$count++]);
  if (strlen($lidnr)==0) $lidnr=0; 

  $query .= $status.','.$wijknaam.','.$voorkeur_sectie.','.$geslacht.',"'.$aanschrijfnaam.'",'.$voegsel.','.$voornamen.','.$straatnaam.','.$huisnummer.',';
  $query .= $postcode.','.$woonplaats.','.$leeftijd.','.$geboorte_datum.','.$geboorte_plaats.','.$burgelijkestaat.','.$kerkelijkestaat.',';
  $query .= $huwelijk_datum.','.$huwelijk_plaats.','. $doop_datum.','.$doop_plaats.','.$kerkelijk_gezindte.','.$belijdenis_datum.',';
  $query .= $belijdenis_plaats.','.$herkomst_datum.','.$herkomst_plaats.','.$mutatie_datum.','.$kerkelijk_relatie.','.$voorkeur_wijknaam.','.$kerkblad.',';
  $query .= $lidnr.',"'.oostpoort_achternaam($aanschrijfnaam).'",';
  $query .= 'SYSDATE() )';

 // $page = $query;
 // $page .= '<br/>';
 // echo $page;

  $result = db_query($query);
  if ($result==1) {
    // Query succesfull
    watchdog('user', 'A member is created in the oostpoort');
    return true;
  }
  else {
    // Query failed
    return false;
  }
}

// ##################################################################################
// THE END
// ##################################################################################
