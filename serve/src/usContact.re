open WorkerCommon;
open DsDataTypes;
open CountryType;

let stringOfUSContactPageData = (p8Data: DsDataTypes.usContact) =>
  Printf.sprintf(
    "
      Contact: %s,
      Contact Relatinoship :%s,
      Address: %s,
      Phone Number :%s,
      Email :%s,",
    stringOfContactPersonOrOrg(p8Data.contactPersonOrOrg),
    stringOfContactRelationship(p8Data.contactRelationship),
    stringOfAddress(p8Data.address),
    p8Data.phoneNum,
    stringOfOptionString("Email", p8Data.email),
  );

let usContactPageData =
    (worksheetValues: worksheetValues)
    : DsDataTypes.usContact => {
  contactPersonOrOrg:
    pluckContactPersonOrOrg(
      pluckUSContactOrg(worksheetValues),
      pluckUSContactSurname(worksheetValues),
      pluckUSContactGiveName(worksheetValues),
    ),
  contactRelationship:
    pluckContactRelationship(
      nonOptionString(
        "US Contact Relationship",
        pluckContactRelationshipRaw(worksheetValues),
      ),
    ),
  address:
    address(
      pluckContactAddressStreet1(worksheetValues),
      pluckContactAddressStreet2(worksheetValues),
      pluckContactAddressCity(worksheetValues),
      pluckContactAddressState(worksheetValues),
      pluckContactAddressPostalCode(worksheetValues),
      pluckContactAddressCountry(worksheetValues),
    ),
  phoneNum:
    nonOptionString(
      "US Contact Phone Numbers",
      pluckContactPhoneNum(worksheetValues),
    ),
  email: pluckContactEmail(worksheetValues),
};
