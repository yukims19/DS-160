open WorkerCommon;
open DsDataTypes;

let personal2PageData = (worksheetValues: worksheetValues) : personal2 => {
  nationality: nationality(pluckNationality(worksheetValues)),
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
