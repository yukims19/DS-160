open WorkerCommon;
open DsDataTypes;
open CountryType;

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

let passportPageData = (worksheetValues: worksheetValues) : passport => {
  passType: pluckPassport(worksheetValues),
  passNum:
    nonOptionString("PASSPORT_NUM", pluckPassportNum(worksheetValues)),
  passBookNum: pluckPassportBookNum(worksheetValues),
  countryIssue:
    nonOptionString(
      "PASSPORT_ISSUED_COUNTRY",
      pluckPassportIssuedCountry(worksheetValues),
    ),
  placeIssue:
    shortAddress(
      pluckPassportIssuedCity(worksheetValues),
      pluckPassportIssuedState(worksheetValues),
      pluckPassportIssuedCountry(worksheetValues),
    ),
  issueDate:
    pluckDate(
      pluckPassportIssuedDay(worksheetValues),
      pluckPassportIssuedMonth(worksheetValues),
      pluckPassportIssuedYear(worksheetValues),
    ),
  expirationDate:
    pluckOptionDate(
      Some(pluckPassportExprieDay(worksheetValues)),
      Some(pluckPassportExprieMonth(worksheetValues)),
      Some(pluckPassportExprieYear(worksheetValues)),
    ),
  lostPass:
    optionListOfLostPassInfo(
      pluckLostPassportNum(worksheetValues),
      pluckLostPassportIssuedCountry(worksheetValues),
      pluckLostPasssportExplain(worksheetValues),
    ),
};
