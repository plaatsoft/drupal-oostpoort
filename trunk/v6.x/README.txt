 Release Notes
 -------------
 12-04-2010 (Week 16)
 - Started programming.
 - Create Google Code repository   
 - Create Ohloh project account
 - Create Information page on PlaatSoft website.
 - Create Drupal project "Oostpoort"
 - Based HTML on standard Drupal style sheet
 - Create datamodel.
 - Convert existing data to mysql format.
 - Start building first pages.

 19-04-2010 (Week 17)
 - Create admin visit page.
 - Add functionality to update visitors data.
 - Replace button by links (based on JavaScript)

 26-04-2010 (Week 18)
 - Created visit pages including history information.
 - Added back links to every page.
 - Bugfix: Solve reload button (Multi insert) issue.
 - Cleanup available data. 
 - Created family pages.
 - Created three reports.
 - Added family and member image upload.
 - Cleanup code. 
	- Move all utilities functions to separate file.
	- Remove not used code and files. 
 - Added CSV member upload.
 	- Create data filter to replace special character with standard UTF-8 characters.
 - Improve multi table select.
 - Bugfix: No "visit wanted" is now working fine.
 - Bugfix: Now member AdrId is also update during CSV upload.
 - Added security access for the visit part.

 04-04-2010 (Week 19)
 - Added date picker.
 - Added Access security for all user type's. 
 - Show more information about family in visit address select page.
 - Show more information about previous visit in visit search page.
 - Show more statistics information on visit admin list page. 
 - Return to old datamodel.
 - Clean available data.
 - BugFix: KerkLidNr is not loaded!
 - BugFix: Member record is not completly update when record is found during CSV upload.
 - BugFix: During address selection, go to detail page, return, selection of address is lost!   
 - Retested everything.
  
To do List
----------
- Add Block info to each address
- Add Block filter in visit address search page.

Nice to have
------------
- Make all sql query safe for sql insertion.
- Add more information to database: Married location, etc.....
- Google map intergration.
- Birthday email notification.
- Bug: Remove addresses with no members after csv upload.  
