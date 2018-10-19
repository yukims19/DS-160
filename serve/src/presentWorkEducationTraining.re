open WorkerCommon;
open DsDataTypes;
open CountryType;
/*
 let stringOfPresentWETPageData = (p9Data: DsDataTypes.presentWET) =>
   Printf.sprintf(
     "
       Father: %s,
       Mother:%s,
       Relatives :%s,",
     stringOfPresentWETMember(p9Data.father),
     stringOfPresentWETMember(p9Data.mother),
     stringOfRelativeMember(p9Data.relative),
   );*/

let presentWETPageData =
    (worksheetValues: worksheetValues)
    : DsDataTypes.presentWET => {
  occupation:
    nonOptionString(
      "Present Occupation",
      pluckPresentOccupation(worksheetValues),
    ),
  presentEmployerInfo:
    pluckPresentEmployerInfo(
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
