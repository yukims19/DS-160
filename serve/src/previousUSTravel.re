open WorkerCommon;
open DsDataTypes;
open CountryType;

/*
 let stringOfPassportPageData = (p4Data: DsDataTypes.passport) =>
   Printf.sprintf(
     "
  Passport Type: %s,
  Passport Number: %s,
  Passport Book Number: %s,
  countryIssue: %s,
  placeIssue: %s,
  isscueDate: %s,
  expirationDate :%s,
  lostPass: %s",
     stringOfPassportType(p4Data.passType),
     p4Data.passNum,
     stringOfOptionString("Passport Book Num", p4Data.passBookNum),
     p4Data.countryIssue,
     stringOfShortAddress(p4Data.placeIssue),
     stringOfDate(p4Data.issueDate),
     stringOfOptionDate(p4Data.expirationDate),
     stringOfLostPassport(p4Data.lostPass),
   );
  */

let previousUSTravelPageData =
    (worksheetValues: worksheetValues)
    : previousUSTravel => {
  lastTravels:
    pluckOptionListPreTravelInfo(
      pluckPreviousVisitDay(worksheetValues),
      pluckPreviousVisitMonth(worksheetValues),
      pluckPreviousVisitYear(worksheetValues),
      pluckPreviousVisiteStayTimeNumber(worksheetValues),
      pluckPreviousVisiteStayTimeUnitRaw(worksheetValues),
    ),
  hasDriverLicense:
    pluckOptionListDriverLicenseInfo(
      pluckDriverLicenseNumber(worksheetValues),
      pluckDriverLicenseNumber(worksheetValues),
    ),
  preVisa:
    pluckPreviouVisa(
      pluckPreviousVisaIssuedDay(worksheetValues),
      pluckPreviousVisaIssuedMonth(worksheetValues),
      pluckPreviousVisaIssuedYear(worksheetValues),
      pluckPreviousVisaNum(worksheetValues),
      pluckIsPreviousVisaSameType(worksheetValues),
      pluckIsPreviousVisaSameLocation(worksheetValues),
      pluckIsPreviousVisaTenPrint(worksheetValues),
      pluckPreviousVisaCancelExpl(worksheetValues),
      pluckPreviousVisaLostYear(worksheetValues),
      pluckPreviousVisaLostExpl(worksheetValues),
    ),
  refused: pluckRefusedExpl(worksheetValues),
  petition: pluckPetitionExpl(worksheetValues),
};
