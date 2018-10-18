open WorkerCommon;
open DsDataTypes;
open CountryType;

let stringOfTravelPageData = (p5Data: DsDataTypes.travel) =>
  Printf.sprintf(
    "
     Purpose to US: %s,
   Travel Plan: %s,
     Person Paying: %s,",
    stringOfPurposeToUS(p5Data.purposeToUS),
    stringOfTravelPlan(p5Data.travelPlan),
    stringOfPersonPaying(p5Data.personPaying),
  );

let travelPageData = (worksheetValues: worksheetValues) : travel => {
  purposeToUS:
    listPurposeInfo(
      pluckPurposeOfTrip(worksheetValues),
      pluckSpecificPurpose(worksheetValues),
    )
    |> nonOptionList("PurposeToUS"),
  travelPlan:
    switch (
      pluckHasTravelPlan(worksheetValues)
      |> nonOptionString("HAS_TRAVEL_PLAN")
    ) {
    | "T" =>
      /*TODO: Need to handle different cases*/
      HasTravelPlan({
        arrivalDate:
          pluckDate(
            pluckTravelArrivalDay(worksheetValues),
            pluckTravelArrivalMonth(worksheetValues),
            pluckTravelArrivalYear(worksheetValues),
          ),
        arrivalFlight: None,
        arrivalCity: "city",
        departureDate:
          pluckDate(
            pluckTravelArrivalDay(worksheetValues),
            pluckTravelArrivalMonth(worksheetValues),
            pluckTravelArrivalYear(worksheetValues),
          ),
        departureFlight: None,
        departureCity: "city",
        visitLocation: ["location list"],
        stayAddress:
          address(
            pluckTravelStayStreet1(worksheetValues),
            pluckTravelStayStreet2(worksheetValues),
            pluckTravelStayCity(worksheetValues),
            pluckTravelStayState(worksheetValues),
            pluckTravelStayZipCode(worksheetValues),
            pluckTravelStayCountry(worksheetValues),
          ),
      })
    | "F" =>
      NoTravelPlan({
        arrivalDate:
          pluckDate(
            pluckTravelArrivalDay(worksheetValues),
            pluckTravelArrivalMonth(worksheetValues),
            pluckTravelArrivalYear(worksheetValues),
          ),
        stayLength:
          pluckTimeLength(
            nonOptionString(
              "STAY_LENGTH_NUMBER",
              pluckTravelStayLenthNumber(worksheetValues),
            )
            |> int_of_string,
            pluckTravelStayLenthUnit(worksheetValues),
          ),
        stayAddress:
          address(
            pluckTravelStayStreet1(worksheetValues),
            pluckTravelStayStreet2(worksheetValues),
            pluckTravelStayCity(worksheetValues),
            pluckTravelStayState(worksheetValues),
            pluckTravelStayZipCode(worksheetValues),
            pluckTravelStayCountry(worksheetValues),
          ),
      })
    | _val =>
      raise(
        Failure(
          "Invalide inpt for Person_Paying: "
          ++ _val
          ++ ". Please enter T or F",
        ),
      )
    },
  personPaying: pluckPersonPaying(worksheetValues),
};
