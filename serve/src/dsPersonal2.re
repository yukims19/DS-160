open WorkerCommon;
open DsDataTypes;
open CountryType;
let stringOfPersonal2PageData = (p2Data: DsDataTypes.personal2) =>
  Printf.sprintf(
    "
Nationality: %s
Other Nationalities: %s
Permanent Residents: %s
National ID: %s
SSN: %s
Tax Id: %s",
    stringOfCountry(p2Data.nationality),
    stringOfOptionListNationalityInfo(p2Data.otherNationality),
    stringOfOptionListCountry(p2Data.permanentResident),
    stringOfOptionString("National ID", p2Data.nationalId),
    stringOfOptionString("Social Security Numbers", p2Data.ssn),
    stringOfOptionString("Tax ID", p2Data.taxId),
  );

let personal2PageData = (worksheetValues: worksheetValues) : personal2 => {
  nationality: nation(pluckNationality(worksheetValues)),
  otherNationality:
    optionListNationalityInfo(
      pluckOtherNationality(worksheetValues),
      pluckOtherPassportNum(worksheetValues),
    ),
  permanentResident:
    optionListCountry(pluckPermanentResident(worksheetValues)),
  nationalId: pluckNationalId(worksheetValues),
  ssn: pluckSNN(worksheetValues),
  taxId: pluckTaxId(worksheetValues),
};
