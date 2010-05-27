 Release Notes
 -------------
 
 10-09-2009 Version v5.x-0.1
 - Member list page.
 - Member detail page.
 - General CSV upload.
 - Added Member image upload.
 - Create Data Model.
 - Proof-of-concept build for Drupal 5.x

 15-09-2009 Version v5.x-0.2
 - Added Family overview page.
 - Added Age report page.
 - Added Jubilee report page.
 - Proof-of-concept build for Drupal 5.x

 10-04-2010 (Week 15) v6.x-0.3
 - Started PHP programming. 
 - Convert proof-of-concept module from Drupal 5 to Drupal 6.
 - Switch from oracle datamodel to mysql datamodel.
 - Build for Drupal 6.x
 - Internal release

 18-04-2010 (Week 16) v6.x-0.4
 - Create Google Code repository.   
 - Create Ohloh project account.
 - Create information page on PlaatSoft website.
 - Create Drupal project "Oostpoort" on drupal.org
 - Based HTML output on standard Drupal Style sheet.
 - Start building first pages.
 - Build for Drupal 6.x
 - Internal release

 25-04-2010 (Week 17) v6.x-0.5
 - Create admin visit page.
 - Added functionality to update visitors data.
 - Replace button by links (based on JavaScript)
 - Build for Drupal 6.x
 - Internal release

 02-05-2010 (Week 18)v6.x-0.6
 - Created visit pages including history information.
 - Added "return" links to previous page.
 - Cleanup available data. 
 - Created family pages.
 - Created three reports.
 - Added image upload.
 - Cleanup code. 
	- Move all utilities functions to separate file.
	- Remove not used code and files. 
 - Added CSV data upload.
 - Added CSV data filter to replace non ASCII character.
 - Added multi table select functionality.
 - Bugfix: No "visit wanted" is now working fine.
 - Bugfix: Now member AdrId is also update during CSV upload.
 - Bugfix: Solve reload button (Multi insert) issue.
 - Build for Drupal 6.x
 - Internal release

 09-05-2010 (Week 19) v6.x-0.7
 - Added jquery date picker.
 - Added access security control based on user type's. 
 - Show more information about family in visit address select page.
 - Show more information about previous visit in visit search page.
 - Show more statistics information on visit admin list page. 
 - Cleanup available data.
 - Added version banner on main menu page.
 - Added latest csv upload date on main menu page.
 - Optimise HTML output for Internet Explorer (6.x, 7.x and 8.x) and Firefox (3.x).
 - Improve two reports.
 - Added functionality to remove members (Drupal admin rights needed) 
 - BugFix: KerkLidNr was not loaded (fixed)!
 - BugFix: Member record was not completly updated during CSV upload.
 - Build for Drupal 6.x  
 - Internal release

 13-05-2010 (Week 20) v6.x-0.8
 - Added BlockId to all address entries.
 - Show only addresses in the block where the visitor is active.
 - Added support for lightbox2 drupal module. Nice photo popup effect!
 - Improve module directory structure.
 - Improve breadcrumb menu. 
 - Improve code syntax and source documentation.
 - Improve form input validation.
 - Make all SQL queries safe for SQL insertion.
 - Added birthday email notification (cron job).
 - Added birthday summary block page.
 - Added picture remove.
 - Bugfixes: Solve some minor issues after first deployment on my demo drupal6 site.
 - Build for Drupal 6.x
 - This is the first beta release for beta testers.
    
21-05-2010 (Week 31) v6.x-0.9
 - Make member picture optional (Admin setting).
 - Show member Id and church Id only for drupal administrator users.
 - Hide Married date and Married state for non visit members.
 - Added functionality to cancel planned visited.
 - Added functionality to adapted a saved (finished) visit report.
 - Build for Drupal 6.x
 - Internal release

26-05-2010 (Week 32) v6.x-1.0
 - Added Dooplid and Belijdenislid information to database and views.
 - Improve Married and Birthday report. 
 - Improve family view.
 - Bugfix: Improve csv output data filtering.
 - Bugfix: Delete member is working again.
 - Bugfix: When new member is added all date fields are now correctly filled.
 - Build for Drupal 6.x
 - First official release for public us!
 
Todo:
 - Modify telefoonnummer in visit screen.
 - Filter novisit in visit address selection. 

Nice to have
------------
- Google map intergration.

Now issues
----------
- Bug: Remove addresses with no members after csv upload.  
