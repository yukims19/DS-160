open WorkerCommon;
open DsDataTypes;
open CountryType;

let stringOfPreviousWETPageData = (p11Data: DsDataTypes.previousWET) =>
  Printf.sprintf(
    "
        Previous Employer Detail: %s,
      Previous Education Detail: %s,",
    stringOfPreviousEmployer(p11Data.previousEmployer),
    stringOfPreviousEducation(p11Data.education),
  );

let previousWETPageData =
    (worksheetValues: worksheetValues)
    : DsDataTypes.previousWET => {
  previousEmployer:
    pluckPreviousEmployer(
      pluckPreviousEmployerName(worksheetValues),
      pluckPreviousEmployerAddress1(worksheetValues),
      pluckPreviousEmployerAddress2(worksheetValues),
      pluckPreviousEmployerAddressCity(worksheetValues),
      pluckPreviousEmployerAddressState(worksheetValues),
      pluckPreviousEmployerAddressPostalCode(worksheetValues),
      pluckPreviousEmployerAddressCountry(worksheetValues),
      pluckPreviousEmployerPhoneNum(worksheetValues),
      pluckPreviousEmployerJob_Title(worksheetValues),
      pluckPreviousEmployerSupervisorSurname(worksheetValues),
      pluckPreviousEmployerSupervisorGiveName(worksheetValues),
      pluckPreviousEmployerStartDay(worksheetValues),
      pluckPreviousEmployerStartMonth(worksheetValues),
      pluckPreviousEmployerStartYear(worksheetValues),
      pluckPreviousEmployerEndDay(worksheetValues),
      pluckPreviousEmployerEndMonth(worksheetValues),
      pluckPreviousEmployerEndYear(worksheetValues),
      pluckPreviousEmployerDuty(worksheetValues),
    ),
  education:
    pluckPreviousEducation(
      pluckPreviousSchoolName(worksheetValues),
      pluckPreviousSchoolAddress1(worksheetValues),
      pluckPreviousSchoolAddress2(worksheetValues),
      pluckPreviousSchoolAddressCity(worksheetValues),
      pluckPreviousSchoolAddressState(worksheetValues),
      pluckPreviousSchoolAddressPostalCode(worksheetValues),
      pluckPreviousSchoolAddressCountry(worksheetValues),
      pluckPreviousSchoolPhoneNum(worksheetValues),
      pluckPreviousSchoolCourseOfStudy(worksheetValues),
      pluckPreviousSchoolStartDay(worksheetValues),
      pluckPreviousSchoolStartMonth(worksheetValues),
      pluckPreviousSchoolStartYear(worksheetValues),
      pluckPreviousSchoolEndDay(worksheetValues),
      pluckPreviousSchoolEndMonth(worksheetValues),
      pluckPreviousSchoolEndYear(worksheetValues),
    ),
};
