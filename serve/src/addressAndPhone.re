open WorkerCommon;
open DsDataTypes;
open CountryType;
let stringOfAddressAndPhonePageData = (p3Data: DsDataTypes.addressAndPhone) =>
  Printf.sprintf(
    "
Home Address:%s,
Mailing Address:%s,
Primary Phone:%s,
Secondary Phone:%s,
Work Phone:%s,
Email:%s",
    stringOfAddress(p3Data.homeAddress),
    stringOfOptionAddress(p3Data.mailingAddress),
    p3Data.primaryPhone,
    stringOfOptionString("Secondary Phone", p3Data.secPhone),
    stringOfOptionString("Work Phone", p3Data.workPhone),
    p3Data.email,
  );

let addressAndPhonePageData =
    (worksheetValues: worksheetValues)
    : addressAndPhone => {
  homeAddress:
    address(
      pluckHomeStreet1(worksheetValues),
      pluckHomeStreet2(worksheetValues),
      pluckHomeCity(worksheetValues),
      pluckHomeState(worksheetValues),
      pluckHomeZipCode(worksheetValues),
      pluckHomeCountry(worksheetValues),
    ),
  mailingAddress:
    optionAddress(
      pluckMailingStreet1(worksheetValues),
      pluckMailingStreet2(worksheetValues),
      pluckMailingCity(worksheetValues),
      pluckMailingState(worksheetValues),
      pluckMailingZipCode(worksheetValues),
      pluckMailingCountry(worksheetValues),
    ),
  primaryPhone:
    nonOptionString("HOME_TEL", pluckPrimaryPhone(worksheetValues)),
  secPhone: pluckSecPhone(worksheetValues),
  workPhone: pluckWorkPhone(worksheetValues),
  email: nonOptionString("EMAIL", pluckEmail(worksheetValues)),
};
