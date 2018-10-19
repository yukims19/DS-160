open WorkerCommon;
open DsDataTypes;
open CountryType;

let stringOfPresentWETPageData = (p10Data: DsDataTypes.presentWET) =>
  Printf.sprintf(
    "
       Occupation: %s,
       Detail: %s,",
    OccupationType.stringOfOccupation(p10Data.occupation),
    stringOfPresentEmployerInfo(p10Data.presentEmployerInfo),
  );

let presentWETPageData =
    (worksheetValues: worksheetValues)
    : DsDataTypes.presentWET => {
  occupation:
    OccupationType.occupationOfStringOccupation(
      nonOptionString(
        "PresentOccupationRaw",
        pluckPresentOccupationRaw(worksheetValues),
      ),
    ),
  presentEmployerInfo:
    pluckPresentEmployerInfo(
      OccupationType.occupationOfStringOccupation(
        nonOptionString(
          "PresentOccupationRaw",
          pluckPresentOccupationRaw(worksheetValues),
        ),
      ),
      pluckPresentOccupationExplain(worksheetValues),
      pluckPresentEmployerName(worksheetValues),
      pluckPresentEmployerAddress1(worksheetValues),
      pluckPresentEmployerAddress2(worksheetValues),
      pluckPresentEmployerAddressCity(worksheetValues),
      pluckPresentEmployerAddressState(worksheetValues),
      pluckPresentEmployerAddressPostalCode(worksheetValues),
      pluckPresentEmployerAddressCountry(worksheetValues),
      pluckPresentEmployerPhoneNum(worksheetValues),
      pluckPresentEmployerStartDay(worksheetValues),
      pluckPresentEmployerStartMonth(worksheetValues),
      pluckPresentEmployerStartYear(worksheetValues),
      pluckPresentEmployerMonthlyIncome(worksheetValues),
      pluckPresentEmployerDuty(worksheetValues),
    ),
};
