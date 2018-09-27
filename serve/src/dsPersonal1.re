open WorkerCommon;
open DsDataTypes;

let stringOfPersonal1PageData = (p1Data: DsDataTypes.personal1) =>
  Printf.sprintf(
    "Surname: %s
Given name: %s
Full name: %s
Gender: %s
Marital Status: %s
Date of birth: %s",
    p1Data.name.surname,
    p1Data.name.givenName,
    stringOfFullName(p1Data.nativeFullName),
    stringOfGender(p1Data.sex),
    stringOfMartialStatus(p1Data.maritalStatus),
    stringOfDate(p1Data.dateOfBirth),
  );

let personal1PageData = (worksheetValues: worksheetValues) : personal1 => {
  name:
    fullName(
      pluckSurname(worksheetValues),
      pluckGivenName(worksheetValues),
    ),
  nativeFullName: pluckNativeFullName(worksheetValues),
  otherNames:
    optionListFullNames(
      pluckAliasSurname(worksheetValues),
      pluckAliasGivenName(worksheetValues),
    ),
  telecodeName:
    optionFullName(
      pluckTelecodeSurname(worksheetValues),
      pluckTelecodeSurname(worksheetValues),
    ),
  sex: pluckGender(worksheetValues),
  maritalStatus: pluckMaritalStatus(worksheetValues),
  dateOfBirth:
    pluckDate(
      pluckP1DOBDay(worksheetValues),
      pluckP1DOBMonth(worksheetValues),
      pluckP1DOBYear(worksheetValues),
    ),
  placeOfBirth:
    shortAddress(
      pluckP1POBCity(worksheetValues),
      pluckP1POBState(worksheetValues),
      pluckP1POBCountry(worksheetValues),
    ),
};
