open WorkerCommon;
open DsDataTypes;
open CountryType;

let stringOfPreviousUSTravelPageData = (p7Data: DsDataTypes.previousUSTravel) =>
  Printf.sprintf(
    "
     Last Travels: %s,
     Driver License :%s,
     Previous Visa: %s,
     Refused :%s,
     Petition :%s,",
    stringOfPreTravelInfo(p7Data.lastTravels),
    stringOfDriverLicense(p7Data.hasDriverLicense),
    stringOfPreVisa(p7Data.preVisa),
    stringOfOptionString("Refused", p7Data.refused),
    stringOfOptionString("petition", p7Data.petition),
  );

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
