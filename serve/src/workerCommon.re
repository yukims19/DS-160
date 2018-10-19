type worksheetValues = list((string, list(string)));

let makePluckerOne = (key, worksheetValues: worksheetValues) => {
  let values =
    try (List.assoc(String.lowercase_ascii(key), worksheetValues)) {
    | Not_found =>
      raise(
        Failure(
          Printf.sprintf(
            "Could not find key in worksheet for pluckerOne: %s",
            key,
          ),
        ),
      )
    };
  switch (values) {
  | [] => None
  | [value] => Some(value)
  | values =>
    raise(
      Failure(
        Printf.sprintf(
          "More than one %s found: [%s]",
          key,
          String.concat(",", values),
        ),
      ),
    )
  };
};

let makePluckerMulti = (key, worksheetValues: worksheetValues) => {
  let values =
    try (List.assoc(String.lowercase_ascii(key), worksheetValues)) {
    | Not_found =>
      raise(
        Failure(
          Printf.sprintf(
            "Could not find key in worksheet for pluckerMulti: %s",
            key,
          ),
        ),
      )
    };
  switch (values) {
  | [] => None
  | values => Some(values)
  };
};

let nonOptionString = (key, value) =>
  switch (value) {
  | Some(realValue) => realValue
  | None => raise(Failure(Printf.sprintf("%s cannot be empty", key)))
  };

let nonOptionList = (key, listValue) =>
  switch (listValue) {
  | Some(realList) => realList
  | None => raise(Failure(Printf.sprintf("%s cannot be empty", key)))
  };

let pluckGivenName = makePluckerOne("GIVEN_NAME");
let pluckSurname = makePluckerOne("SURNAME");
let pluckGenderRaw = makePluckerOne("GENDER");
let pluckMaritalStatusRaw = makePluckerOne("MARITAL_STATUS");

let pluckMaritalStatus = worksheetValues : DsDataTypes.marital =>
  switch (pluckMaritalStatusRaw(worksheetValues)) {
  | Some(marital) =>
    switch (marital) {
    | "Married" => Married
    | "Single" => Single
    | "Common" => Common
    | "Civil" => Civil
    | "Widowed" => Widowed
    | "Divorced" => Divorced
    | "Seperate" => Seperate
    | "Other" => Other
    | _ =>
      raise(
        Failure(
          "Invalid value for MARITAL_STATUS. Please enter Married, Single, Common, Civil, Widowed, Divorced, Seperate or Other",
        ),
      )
    }
  | None => raise(Failure("MARITAL_STATUS cannot be null"))
  };

let pluckGender = worksheetValues =>
  DsDataTypes.(
    switch (pluckGenderRaw(worksheetValues)) {
    | Some(gender) =>
      switch (gender) {
      | "F" => Female
      | "M" => Male
      | "FEMALE" => Female
      | "MALE" => Male
      | other =>
        raise(
          Failure(
            Printf.sprintf(
              "GENDER value '%s' is not valid for our system. Please enter 'F' or 'M'",
              other,
            ),
          ),
        )
      }
    | None => raise(Failure("Gender cannot be null"))
    }
  );

let fullName = (surname, givenName) : DsDataTypes.name => {
  surname: nonOptionString("SURNAME", surname),
  givenName: nonOptionString("GIVEN_NAME", givenName),
};

let pluckP1DOBDay = makePluckerOne("DATE_OF_BIRTHDay");
let pluckP1DOBMonth = makePluckerOne("DATE_OF_BIRTHMonth");
let pluckP1DOBYear = makePluckerOne("DATE_OF_BIRTHYear");

let pluckDate = (day, month, year) : DsDataTypes.date => {
  day: nonOptionString("day", day),
  month:
    switch (month) {
    | Some(month) =>
      switch (month) {
      | "Jan" => Jan
      | "Feb" => Feb
      | "Mar" => Mar
      | "Apr" => Apr
      | "May" => May
      | "Jun" => Jun
      | "Jul" => Jul
      | "Aug" => Aug
      | "Sep" => Sep
      | "Oct" => Oct
      | "Nov" => Nov
      | "Dec" => Dec
      /* XXX: Might not be a good idea, maybe force the user to use the above
         format instead */
      | "1" => Jan
      | "2" => Feb
      | "3" => Mar
      | "4" => Apr
      | "5" => May
      | "6" => Jun
      | "7" => Jul
      | "8" => Aug
      | "9" => Sep
      | "10" => Oct
      | "11" => Nov
      | "12" => Dec
      | _ =>
        raise(
          Failure(
            "Invalid value for month. Please enter in format Jan, Feb, Mar ....",
          ),
        )
      }
    | None => raise(Failure("Surname cannot be null"))
    },
  year: nonOptionString("year", year),
};

let pluckOptionDate = (day, month, year) =>
  switch (day, month, year) {
  | (None, None, None) => None
  | (Some(day), Some(month), Some(year)) =>
    Some(pluckDate(Some(day), Some(month), Some(year)))
  | _ =>
    raise(
      Failure("Date, Month, Year should all have some value or all be empty"),
    )
  };
let pluckListDate = (day, month, year) =>
  List.length(day) == List.length(month)
  && List.length(month) == List.length(year) ?
    List.mapi(
      (idx, day) =>
        pluckDate(
          Some(day),
          Some(List.nth(month, idx)),
          Some(List.nth(year, idx)),
        ),
      day,
    ) :
    raise(
      Failure(
        "The Following Fields must be in same length:
         day, month,year.
         Please enter N/A for unknown fields",
      ),
    );
let pluckP1POBCity = makePluckerOne("PLACE_OF_BIRTH_CITY");
let pluckP1POBState = makePluckerOne("PLACE_OF_BIRTH_STATE_PROVINCE");
let pluckP1POBCountry = makePluckerOne("PLACE_OF_BIRTH_CNTRY");

let shortAddress = (city, state, country) : DsDataTypes.shortAddress => {
  city: nonOptionString("CITY", city),
  state,
  country:
    switch (country) {
    | Some(country) =>
      CountryType.countryTypeOfFullCountryName(
        String.capitalize_ascii(country),
      )
    | None => raise(Failure("COUNTRY cannot be null"))
    },
};

let pluckNativeFullName = makePluckerOne("FULL_NAME_NATIVE");
let pluckTelecodeSurname = makePluckerOne("TELECODE_SURNAME");
let pluckTelecodeGivenName = makePluckerOne("TELECODE_GIVEN_NAME");

let optionFullName = (surname, givenName) =>
  switch (surname) {
  | Some(surname) =>
    switch (givenName) {
    | Some(givenName) => Some({surname, givenName}: DsDataTypes.name)
    | None => None
    }
  | None => None
  };

let pluckAliasSurname = makePluckerMulti("ALIAS_SURNAME");
let pluckAliasGivenName = makePluckerMulti("ALIAS_GIVEN_NAME");

/*TODO: NEED to Construct list here*/
let optionListFullNames = (aliasSurnames, aliasGivenNames) =>
  switch (aliasGivenNames, aliasSurnames) {
  | (None, None) => None
  | (Some(givenNames), Some(surnames)) =>
    Some(
      List.map2(
        (givenName, surname) => ({surname, givenName}: DsDataTypes.name),
        givenNames,
        surnames,
      ),
    )
  | (Some(_), None)
  | (None, Some(_)) =>
    raise(
      Failure(
        "ALIAS_SURNAME and ALIAS_GIVEN_NAME must either both be absent or both be present",
      ),
    )
  };

let pluckNationality = makePluckerOne("NATIONALITY");
let nation = nation =>
  switch (nation) {
  | Some(nation) =>
    String.map(a => Char.uppercase_ascii(a), nation)
    |> CountryType.countryTypeOfFullCountryName

  | None => raise(Failure("Nationality cannot be null"))
  };

let pluckOtherNationality = makePluckerMulti("OTHER_NATIONALITY");
let pluckOtherPassportNum = makePluckerMulti("OTHER_PASSPORT_NUM");
let optionListNationalityInfo = (nation, passport) =>
  switch (nation, passport) {
  | (None, None) => None
  | (Some(nation), Some(passport)) =>
    Some(
      List.map2(
        (nation, passport) => {
          let passportNum =
            switch (passport) {
            | "N/A" => None
            | _ => Some(passport)
            };
          let nationality =
            String.map(a => Char.uppercase_ascii(a), nation)
            |> CountryType.countryTypeOfFullCountryName;

          ({nationality, passportNum}: DsDataTypes.nationalityInfo);
        },
        nation,
        passport,
      ),
    )
  | (Some(_), None)
  | (None, Some(_)) =>
    raise(
      Failure(
        "OTHER_NATIONALITY and OTHER_PASSPORT_NUM must either both be absent or both be present. Please enter N/A for unknow passport number",
      ),
    )
  };
let pluckPermanentResident = makePluckerMulti("PERMANENT_RESIDENT");
let optionListCountry = countries =>
  switch (countries) {
  | Some(countries) =>
    Some(
      List.map(
        country =>
          CountryType.countryTypeOfFullCountryName(
            String.capitalize_ascii(country),
          ),
        countries,
      ),
    )
  | None => None
  };
let pluckNationalId = makePluckerOne("NATIONAL_ID");
let pluckSNN = makePluckerOne("SSN");
let pluckTaxId = makePluckerOne("TAX_ID");

let pluckHomeStreet1 = makePluckerOne("HOME_ADDR_LN1");
let pluckHomeStreet2 = makePluckerOne("HOME_ADDR_LN2");
let pluckHomeCity = makePluckerOne("HOME_ADDR_CITY");
let pluckHomeState = makePluckerOne("HOME_ADDR_STATE");
let pluckHomeZipCode = makePluckerOne("HOME_ADDR_POSTAL_CD");
let pluckHomeCountry = makePluckerOne("HOME_ADDR_COUNTRY");
let address =
    (streetL1, streetL2, city, state, zipCode, country)
    : DsDataTypes.address => {
  streetL1: nonOptionString("StreetL1", streetL1),
  streetL2,
  city: nonOptionString("CITY", city),
  state,
  zipCode,
  country: nation(country),
};

let listAddress =
    (streetL1s: list(string), streetL2s, citys, states, zipCodes, countrys)
    : list(DsDataTypes.address) => {
  let listLength = List.length(streetL1s);
  if (listLength == List.length(streetL1s)
      && listLength == List.length(streetL2s)
      && listLength == List.length(citys)
      && listLength == List.length(states)
      && listLength == List.length(zipCodes)
      && listLength == List.length(countrys)) {
    List.mapi(
      (idx, streetL1) =>
        address(
          switch (streetL1) {
          | "N/A" => raise(Failure("StreetL1 cannot be N/A"))
          | _street => Some(_street)
          },
          switch (List.nth(streetL2s, idx)) {
          | "N/A" => None
          | _street => Some(_street)
          },
          switch (List.nth(citys, idx)) {
          | "N/A" => raise(Failure("City cannot be N/A"))
          | _street => Some(_street)
          },
          switch (List.nth(states, idx)) {
          | "N/A" => None
          | _state => Some(_state)
          },
          switch (List.nth(zipCodes, idx)) {
          | "N/A" => None
          | _code => Some(_code)
          },
          switch (List.nth(countrys, idx)) {
          | "N/A" => raise(Failure("Country cannot be N/A"))
          | _street => Some(_street)
          },
        ),
      streetL1s,
    );
  } else {
    raise(
      Failure(
        "The Following Fields must be in same length:
         streetL1, streetL2, city, state, zipCode, country.
        Please enter N/A for unknown fields",
      ),
    );
  };
};

let pluckMailingStreet1 = makePluckerOne("MAILING_ADDR_LN1");
let pluckMailingStreet2 = makePluckerOne("MAILING_ADDR_LN2");
let pluckMailingCity = makePluckerOne("MAILING_ADDR_CITY");
let pluckMailingState = makePluckerOne("MAILING_ADDR_STATE");
let pluckMailingZipCode = makePluckerOne("MAILING_ADDR_POSTAL_CD");
let pluckMailingCountry = makePluckerOne("MAILING_ADDR_COUNTRY");
let optionAddress = (streetL1, streetL2, city, state, zipCode, country) =>
  switch (streetL1) {
  | Some(_street) =>
    Some(
      {
        streetL1: nonOptionString("StreetL1", streetL1),
        streetL2,
        city: nonOptionString("CITY", city),
        state,
        zipCode,
        country: nation(country),
      }: DsDataTypes.address,
    )
  | None => None
  };

let pluckPrimaryPhone = makePluckerOne("HOME_TEL");
let pluckSecPhone = makePluckerOne("MOBILE_TEL");
let pluckWorkPhone = makePluckerOne("HOME_TEL");
let pluckEmail = makePluckerOne("EMAIL");

let pluckPassportRaw = makePluckerOne("PASSPORT_TYPE");
let pluckPassport = worksheetValues : DsDataTypes.passType =>
  switch (pluckPassportRaw(worksheetValues)) {
  | Some(passType) =>
    switch (passType) {
    | "Regular" => Regular
    | "Official" => Official
    | "Diplomatic" => Diplomatic
    | "Laissez" => Laissez
    | "Other" => Other
    | _value =>
      raise(
        Failure(
          "Invalide Passport type: "
          ++ _value
          ++ ". Please enter Regular, Official, Diplomatic, Laissez or Other",
        ),
      )
    }
  | None => raise(Failure("PASSPORT_TYPE cannot be empty"))
  };

let pluckPassportNum = makePluckerOne("PASSPORT_NUM");
let pluckPassportBookNum = makePluckerOne("PASSPORT_BOOK_NUM");
let pluckPassportIssuedCountry = makePluckerOne("PASSPORT_ISSUED_COUNTRY");
let pluckPassportIssuedCity = makePluckerOne("PASSPORT_ISSUED_IN_CITY");
let pluckPassportIssuedState = makePluckerOne("PASSPORT_ISSUED_IN_STATE");
let pluckPassportIssuedCountry = makePluckerOne("PASSPORT_ISSUED_IN_COUNTRY");

let pluckPassportIssuedDay = makePluckerOne("PASSPORT_ISSUED_DAY");
let pluckPassportIssuedMonth = makePluckerOne("PASSPORT_ISSUED_MONTH");
let pluckPassportIssuedYear = makePluckerOne("PASSPORT_ISSUED_YEAR");

let pluckPassportExprieDay = makePluckerOne("PASSPORT_EXPIRE_DAY");
let pluckPassportExprieMonth = makePluckerOne("PASSPORT_EXPIRE_MONTH");
let pluckPassportExprieYear = makePluckerOne("PASSPORT_EXPIRE_YEAR");

let pluckLostPassportNum = makePluckerMulti("LOST_PASSPORT_NUM");
let pluckLostPassportIssuedCountry =
  makePluckerMulti("LOST_PASSPORT_ISSUED_COUNTRY");
let pluckLostPasssportExplain = makePluckerMulti("LOST_PASSPORT_EXPLAIN");

let optionListOfLostPassInfo =
    (passNum, countryIssue, explain)
    : option(list(DsDataTypes.lostPassInfo)) =>
  switch (passNum, countryIssue, explain) {
  | (None, None, None) => None
  | (Some(num), Some(country), Some(explain)) =>
    if (List.length(num) == List.length(country)
        && List.length(country) == List.length(explain)) {
      let numAndCountryList =
        List.map2(
          (num, country) => {
            let passportNum =
              switch (num) {
              | "N/A" => None
              | _ => Some(num)
              };
            let nation =
              String.map(a => Char.uppercase_ascii(a), country)
              |> CountryType.countryTypeOfFullCountryName;

            ({passportNum, nationality: nation}: DsDataTypes.nationalityInfo);
          },
          num,
          country,
        );
      let numAndCountryAndExplainList =
        List.map2(
          (numAndCountry: DsDataTypes.nationalityInfo, explain) => (
            {
              passNum: numAndCountry.passportNum,
              countryIssue: numAndCountry.nationality,
              explain,
            }: DsDataTypes.lostPassInfo
          ),
          numAndCountryList,
          explain,
        );
      Some(numAndCountryAndExplainList);
    } else {
      raise(
        Failure(
          "Lost passport number, issued country and explain needs to be same length",
        ),
      );
    }
  | _ =>
    raise(
      Failure(
        "OTHER_NATIONALITY and OTHER_PASSPORT_NUM must either both be absent or both be present. Please enter N/A for unknow passport number",
      ),
    )
  };

let pluckPurposeOfTripRow = makePluckerMulti("PURPOSE_OF_TRIP");
let pluckPurposeOfTrip = worksheetValues : option(list(DsDataTypes.purpose)) =>
  switch (pluckPurposeOfTripRow(worksheetValues)) {
  | Some(purposeOfTrip) =>
    Some(
      List.map(
        purposeOfTrip =>
          switch (purposeOfTrip) {
          | "A" => (A: DsDataTypes.purpose)
          | "B" => B
          | "C" => C
          | "D" => D
          | "E" => E
          | "F" => F
          | "G" => G
          | _value =>
            raise(
              Failure(
                "Invalide type for Person_Paying: "
                ++ _value
                ++ ". Please enter Self, OtherPerson, Employee, USEmployee or OtherOrg.",
              ),
            )
          },
        purposeOfTrip,
      ),
    )
  | None => raise(Failure("PERSON_PAYING cannot be empty"))
  };

let pluckSpecificPurpose = makePluckerMulti("SPECIFIC_PURPOSE");

let listPurposeInfo = (purpose, specificPurpose) =>
  switch (purpose, specificPurpose) {
  | (Some(purpose), Some(specificPurpose)) =>
    Some(
      List.map2(
        (purpose, specificPurpose) => (
          {purpose, specify: specificPurpose}: DsDataTypes.purposeInfo
        ),
        purpose,
        specificPurpose,
      ),
    )
  | _ =>
    raise(
      Failure("PURPOSE_OF_TRIP and SPECIFIC_PURPOSE must both be present"),
    )
  };

let pluckHasTravelPlan = makePluckerOne("HAS_TRAVEL_PLAN");

let pluckTravelArrivalDay = makePluckerOne("TRAVEL_ARRIVAL_DAY");
let pluckTravelArrivalMonth = makePluckerOne("TRAVEL_ARRIVAL_MONTH");
let pluckTravelArrivalYear = makePluckerOne("TRAVEL_ARRIVAL_YEAR");

let pluckTravelStayLenthNumber = makePluckerOne("TRAVEL_LENGTH_NUMBER");
let pluckTravelStayLenthUnitRaw = makePluckerOne("TRAVEL_LENGTH_UNIT");
let pluckTravelStayLenthUnit = worksheetValues : DsDataTypes.timeUnit =>
  switch (pluckTravelStayLenthUnitRaw(worksheetValues)) {
  | Some(timeUnit) =>
    switch (timeUnit) {
    | "Y" => Y
    | "M" => M
    | "W" => W
    | "D" => D
    | _value =>
      raise(
        Failure(
          "Invalide type for Time Unit: "
          ++ _value
          ++ ". Please enter Y, M, W, or D.",
        ),
      )
    }
  | None => raise(Failure("Time Unit cannot be empty"))
  };

let pluckTimeLength = (number, timeUnit) : DsDataTypes.timeLength => {
  number,
  timeUnit,
};

let pluckTravelStayStreet1 = makePluckerOne("TRAVEL_STAY_ADDR_LN1");
let pluckTravelStayStreet2 = makePluckerOne("TRAVEL_STAY_ADDR_LN2");
let pluckTravelStayCity = makePluckerOne("TRAVEL_STAY_ADDR_CITY");
let pluckTravelStayState = makePluckerOne("TRAVEL_STAY_ADDR_STATE");
let pluckTravelStayZipCode = makePluckerOne("TRAVEL_STAY_ADDR_POSTAL_CD");
let pluckTravelStayCountry = makePluckerOne("TRAVEL_STAY_ADDR_COUNTRY");

let pluckPersonPayingRaw = makePluckerOne("PERSON_PAYING");
let pluckPersonPaying = worksheetValues : DsDataTypes.personPaying =>
  switch (pluckPersonPayingRaw(worksheetValues)) {
  | Some(personPaying) =>
    switch (personPaying) {
    | "Self" => Self
    | "OtherPerson" => OtherPerson
    | "Employee" => Employee
    | "USEmployee" => USEmployee
    | "OtherOrg" => OtherOrg
    | _value =>
      raise(
        Failure(
          "Invalide type for Person_Paying: "
          ++ _value
          ++ ". Please enter Self, OtherPerson, Employee, USEmployee or OtherOrg.",
        ),
      )
    }
  | None => raise(Failure("PERSON_PAYING cannot be empty"))
  };

let pluckTravelCompanionGroupName =
  makePluckerOne("Travel_Companion_Group_Name");
let pluckTravelCompanionSurname = makePluckerOne("Travel_Companion_Surname");
let pluckTravelCompanionGiveName =
  makePluckerOne("Travel_Companion_Given_Name");
let pluckTravelCompanionRelationshipRaw =
  makePluckerOne("Travel_Companion_Relationship");
let pluckTravelCompanionRelationship =
    relationshipRaw
    : DsDataTypes.relationship =>
  switch (relationshipRaw) {
  | None =>
    raise(Failure("If Group_Name is null, PERSON_PAYING cannot be empty"))
  | Some(input) =>
    switch (input) {
    | "Parent" => Parent
    | "Spouse" => Spouse
    | "Child" => Child
    | "Relative" => Relative
    | "Friend" => Friend
    | "Bussiness" => Bussiness
    | "Other" => Other
    | _ =>
      raise(
        Failure(
          "Invalid value for month. Please enter  Parent,
            Spouse
            Child
            Relative
            Friend
            Bussiness or
            Other",
        ),
      )
    }
  };
let pluckCompanion =
    (groupName, surname, givenName, relationshipRaw)
    : option(DsDataTypes.companion) =>
  switch (groupName, surname, givenName, relationshipRaw) {
  | (None, None, None, None) => None
  | (Some(groupName), _, _, _) => Some(Org(groupName))
  | (None, Some(surname), Some(givenName), Some(relationshipRaw)) =>
    Some(
      PersonAndRelationship({
        name: fullName(Some(surname), Some(givenName)),
        relationship:
          pluckTravelCompanionRelationship(Some(relationshipRaw)),
      }),
    )
  | _ =>
    raise(
      Failure(
        "If has travel companions, please enter either Group_Name, or Surname,Given_Name, Relationship. If no comapnions, please keep all the fields empty",
      ),
    )
  };

let pluckPreviousVisitDay = makePluckerMulti("Prev_US_Visit_Day");
let pluckPreviousVisitMonth = makePluckerMulti("Prev_US_Visit_Month");
let pluckPreviousVisitYear = makePluckerMulti("Prev_US_Visit_Year");

let pluckPreviousVisiteStayTimeNumber =
  makePluckerMulti("Prev_US_Visit_Stay_Time_Number");
let pluckPreviousVisiteStayTimeUnitRaw =
  makePluckerMulti("Prev_US_Visit_Stay_Time_Unit");

let pluckListTimeUnit = listTimeUnitRaw =>
  List.map(
    timeUnit => (
      switch (timeUnit) {
      | "Y" => Y
      | "M" => M
      | "W" => W
      | "D" => D
      | _value =>
        raise(
          Failure(
            "Invalide type for Time Unit: "
            ++ _value
            ++ ". Please enter Y, M, W, or D.",
          ),
        )
      }: DsDataTypes.timeUnit
    ),
    listTimeUnitRaw,
  );

let pluckListTimeLength =
    (numbers, timeUnitsRaw)
    : list(DsDataTypes.timeLength) =>
  List.map2(
    (number, timeUnit) => (
      {number: int_of_string(number), timeUnit}: DsDataTypes.timeLength
    ),
    numbers,
    pluckListTimeUnit(timeUnitsRaw),
  );

let pluckOptionListPreTravelInfo =
    (days, months, years, timeNumbers, timeUnitsRaw) =>
  switch (days, months, years, timeNumbers, timeUnitsRaw) {
  | (None, None, None, None, None) => None
  | (
      Some(days),
      Some(months),
      Some(years),
      Some(timeNumbers),
      Some(timeUnitsRaw),
    ) =>
    let listArrivalDate = pluckListDate(days, months, years);
    let listTimeLength = pluckListTimeLength(timeNumbers, timeUnitsRaw);
    List.length(listArrivalDate) === List.length(listTimeLength) ?
      Some(
        List.map2(
          (arrivalDate, timeLength) => (
            {arrivalDate, timeLength}: DsDataTypes.preTravelInfo
          ),
          listArrivalDate,
          listTimeLength,
        ),
      ) :
      raise(
        Failure("arrivalDate and driverLicenseInfo must have same length"),
      );
  | _ =>
    raise(
      Failure(
        "Fields in arrivalDate and driverLicenseInfo must have same length",
      ),
    )
  };

let pluckDriverLicenseNumber = makePluckerMulti("US_Driver_license_Num");
let pluckDriverLicenseNumber = makePluckerMulti("US_Driver_license_State");

let pluckOptionListDriverLicenseInfo = (licenseNumbers, licenseStates) =>
  switch (licenseNumbers, licenseStates) {
  | (None, None) => None
  | (Some(licenseNumbers), Some(licenseStates)) =>
    List.length(licenseNumbers) === List.length(licenseStates) ?
      Some(
        List.map2(
          (licenseNum, state) => (
            {
              licenseNum: licenseNum == "N/A" ? None : Some(licenseNum),
              state,
            }: DsDataTypes.driverLicenseInfo
          ),
          licenseNumbers,
          licenseStates,
        ),
      ) :
      raise(
        Failure(
          "License Number and License State must have same length. If License Number is unknown, please enter N/A",
        ),
      )
  | _ =>
    raise(
      Failure(
        "License Number and License State must have same length. If License Number is unknown, please enter N/A",
      ),
    )
  };

let pluckOptionStringToBool = value =>
  switch (value) {
  | Some(value) => bool_of_string(value)
  | None => raise(Failure(Printf.sprintf("Bool Cannot be empty")))
  };

let pluckPreviousVisaIssuedDay = makePluckerOne("PREV_VISA_ISSUED_Day");
let pluckPreviousVisaIssuedMonth = makePluckerOne("PREV_VISA_ISSUED_Month");
let pluckPreviousVisaIssuedYear = makePluckerOne("PREV_VISA_ISSUED_Year");

let pluckPreviousVisaNum = makePluckerOne("PREV_VISA_Num");
let pluckIsPreviousVisaSameType = makePluckerOne("PREV_VISA_Same_Type");
let pluckIsPreviousVisaSameLocation = makePluckerOne("PREV_VISA_Same_Cntry");
let pluckIsPreviousVisaTenPrint = makePluckerOne("PREV_VISA_TEN_print");
let pluckPreviousVisaCancelExpl = makePluckerOne("PREV_VISA_Cancelled");

let pluckPreviousVisaLostYear = makePluckerOne("PREV_VISA_LOST_Year");
let pluckPreviousVisaLostExpl = makePluckerOne("PREV_VISA_LOST_Explain");
let pluckLostVisaInfo = (year, expl) : option(DsDataTypes.lostVisaInfo) =>
  switch (year, expl) {
  | (None, None) => None
  | (Some(year), Some(expl)) => Some({year, explain: expl})
  | _ => raise(Failure("Year and Explain must both be present or empty"))
  };

let pluckPreviouVisa =
    (
      day,
      month,
      year,
      visaNum,
      isSameType,
      isSameLocation,
      isTenPrint,
      lostYear,
      lostExpl,
      visaCancelExpl,
    )
    : option(DsDataTypes.visaInfo) =>
  switch (day, month, year) {
  | (Some(day), Some(month), Some(year)) =>
    Some({
      issueDate: pluckDate(Some(day), Some(month), Some(year)),
      visaNum,
      sameVisa: pluckOptionStringToBool(isSameType),
      sameLocation: pluckOptionStringToBool(isSameLocation),
      tenPrint: pluckOptionStringToBool(isTenPrint),
      lost: pluckLostVisaInfo(lostYear, lostExpl),
      cancel: visaCancelExpl,
    })
  | (None, None, None) => None
  | _ =>
    raise(Failure("Day, Month, Year must be present for previous visa info"))
  };

let pluckRefusedExpl = makePluckerOne("PREV_Visa_Refused_Expl");
let pluckPetitionExpl = makePluckerOne("PREV_Visa_Petition_Expl");

let pluckUSContactSurname = makePluckerOne("US_Contact_SURNAME");
let pluckUSContactGiveName = makePluckerOne("US_Contact_Given_name");
let pluckUSContactOrg = makePluckerOne("US_Contact_Org");

let pluckContactPersonOrOrg =
    (org, surname, givenName)
    : DsDataTypes.contactPersonOrOrg =>
  switch (org, surname, givenName) {
  | (Some(org), _, _) => Org(org)
  | (None, Some(surname), Some(givenName)) =>
    Person(fullName(Some(surname), Some(givenName)))
  | _ =>
    raise(
      Failure("Please enter either Org Name or Person Surname and Givename"),
    )
  };

let pluckContactRelationshipRaw = makePluckerOne("US_Contact_Relationship");

let pluckContactRelationship =
    relationshipRaw
    : DsDataTypes.contactRelationship =>
  switch (relationshipRaw) {
  | "Relative" => Relative
  | "Spouse" => Spouse
  | "Friend" => Friend
  | "Bussiness" => Bussiness
  | "Employer" => Employer
  | "School" => School
  | "Other" => Other
  | _ =>
    raise(
      Failure(
        "Invalid value for relatinoship. Please enter
          Relative,
          Spouse,
          Friend,
          Bussiness,
          Employer,
          School or
          Other",
      ),
    )
  };

let pluckContactAddressStreet1 = makePluckerOne("US_Contact_ADDR_LN1");
let pluckContactAddressStreet2 = makePluckerOne("US_Contact_ADDR_LN2");
let pluckContactAddressCity = makePluckerOne("US_Contact_ADDR_City");
let pluckContactAddressState = makePluckerOne("US_Contact_ADDR_State");
let pluckContactAddressPostalCode =
  makePluckerOne("US_Contact_ADDR_Postal_CD");
let pluckContactAddressCountry = makePluckerOne("US_Contact_ADDR_Country");

let pluckContactPhoneNum = makePluckerOne("US_Contact_HOME_TEL");
let pluckContactEmail = makePluckerOne("US_Contact_EMAIL_ADDR");

let pluckFatherSurname = makePluckerOne("FATHER_SURNAME");
let pluckFatherGiveName = makePluckerOne("FATHER_Given_Name");

let pluckFatherDOBDate = makePluckerOne("Father_Date_Of_Birth_Day");
let pluckFatherDOBMonth = makePluckerOne("Father_Date_Of_Birth_Month");
let pluckFatherDOBYear = makePluckerOne("Father_Date_Of_Birth_Year");
let pluckFatherStayStatus = makePluckerOne("FATHER_Stay_STATUS");

let pluckMotherSurname = makePluckerOne("MOTHER_SURNAME");
let pluckMotherGiveName = makePluckerOne("MOTHER_Given_Name");

let pluckMotherDOBDate = makePluckerOne("Mother_Date_Of_Birth_Day");
let pluckMotherDOBMonth = makePluckerOne("Mother_Date_Of_Birth_Month");
let pluckMotherDOBYear = makePluckerOne("Mother_Date_Of_Birth_Year");
let pluckMotherStayStatus = makePluckerOne("MOTHER_Stay_STATUS");

let pluckStayStatus = stayStatus : DsDataTypes.stayStatus =>
  switch (stayStatus) {
  | "Citizen" => Citizen
  | "LPR" => LPR
  | "NonImmigrant" => NonImmigrant
  | "Other" => Other
  | _ =>
    raise(
      Failure(
        "Invalid value for Stay Status. Please enter
        Citizen,
        LPR,
        NonImmigrant or
        Other",
      ),
    )
  };
let pluckFamilyMember =
    (surname, givenName, day, month, year, stayStatusRaw)
    : DsDataTypes.familyMember => {
  surname,
  givenName,
  dateOfBirth: pluckOptionDate(day, month, year),
  stayStatus:
    switch (stayStatusRaw) {
    | None => None
    | Some(usStatusRaw) => Some(pluckStayStatus(usStatusRaw))
    },
};

let pluckRelativeSurname = makePluckerMulti("RELATIVE_SURNAME");
let pluckRelativeGiveName = makePluckerMulti("RELATIVE_Given_Name");
let pluckRelativeRelationshipRaw = makePluckerMulti("RELATIVE_Relationship");
let pluckRelativeStayStatus = makePluckerMulti("RELATIVE_Stay_STATUS");
let pluckRelativeRelationship =
    relativeRelationshipRaw
    : DsDataTypes.relativeRelationship =>
  switch (relativeRelationshipRaw) {
  | "Sibling" => Sibling
  | "Spouse" => Spouse
  | "Child" => Child
  | "Fiance" => Fiance
  | _ =>
    raise(
      Failure(
        "Invalid value for Relative Relationship. Please enter       Sibling
        Spouse
        Child or
        Fiance",
      ),
    )
  };

let pluckOptionListRelativeMember =
    (surnames, givenNames, reltionshipsRaw, stayStatuses)
    : option(list(DsDataTypes.relativeMember)) =>
  switch (surnames, givenNames, reltionshipsRaw, stayStatuses) {
  | (None, None, None, None) => None
  | (
      Some(surnames),
      Some(givenNames),
      Some(reltionshipsRaw),
      Some(stayStatuses),
    ) =>
    List.(
      length(surnames) == length(givenNames)
      && length(givenNames) == length(reltionshipsRaw)
      && length(reltionshipsRaw) == length(stayStatuses)
    ) ?
      Some(
        List.mapi(
          (idx, surname) => (
            {
              name:
                fullName(Some(surname), Some(List.nth(givenNames, idx))),
              relativeRelationship:
                pluckRelativeRelationship(List.nth(reltionshipsRaw, idx)),
              stayStatus: pluckStayStatus(List.nth(stayStatuses, idx)),
            }: DsDataTypes.relativeMember
          ),
          surnames,
        ),
      ) :
      raise(
        Failure("Name, Relationship, and Stay Status must be in same length"),
      )

  | _ =>
    raise(
      Failure("Name, Relationship, and Stay Status must be in same length"),
    )
  };

let pluckPresentOccupationRaw = makePluckerOne("Present_Occupation");
let pluckPresentOccupationExplain = makePluckerOne("Present_Occupation_Expl");

let pluckPresentEmployerName = makePluckerOne("Present_Employer_Name");
let pluckPresentEmployerAddress1 = makePluckerOne("Present_Employer_ADDR1");
let pluckPresentEmployerAddress2 = makePluckerOne("Present_Employer_ADDR2");
let pluckPresentEmployerAddressCity =
  makePluckerOne("Present_Employer_ADDR_City");
let pluckPresentEmployerAddressState =
  makePluckerOne("Present_Employer_ADDR_State");
let pluckPresentEmployerAddressPostalCode =
  makePluckerOne("Present_Employer_ADDR_Postal_CD");
let pluckPresentEmployerAddressCountry =
  makePluckerOne("Present_Employer_ADDR_Country");

let pluckPresentEmployerPhoneNum = makePluckerOne("Present_Employer_TEL");
let pluckPresentEmployerStartDay =
  makePluckerOne("Present_Employer_Start_Day");
let pluckPresentEmployerStartMonth =
  makePluckerOne("Present_Employer_Start_Month");
let pluckPresentEmployerStartYear =
  makePluckerOne("Present_Employer_Start_Year");
let pluckPresentEmployerMonthlyIncome =
  makePluckerOne("Present_Employer_MONTHLY_SALARY");
let pluckPresentEmployerDuty = makePluckerOne("Present_Employer_Duty");

let pluckPresentEmployerInfo =
    (
      occupationType,
      explain,
      employerName,
      streetL1,
      streetL2,
      city,
      state,
      zipCode,
      country,
      phoneNum,
      day,
      month,
      year,
      income,
      duty,
    )
    : DsDataTypes.presentEmployerInfo => {
  let presentEmployer: DsDataTypes.presentEmployer = {
    employerName: nonOptionString("Employer Name", employerName),
    employerAddress:
      address(streetL1, streetL2, city, state, zipCode, country),
    phoneNum: nonOptionString("Employer Name", phoneNum),
    startDate: pluckDate(day, month, year),
    monthlyIncome:
      switch (income) {
      | Some(income) => Some(int_of_string(income))
      | None => None
      },
    duty: nonOptionString("Duty", duty),
  };
  switch ((occupationType: OccupationType.occupation)) {
  | OTHER =>
    ExplainAndEmployer({
      explain:
        switch (explain) {
        | Some(expl) => expl
        | None =>
          raise(Failure("Please explain if your occupation is OTHER"))
        },
      presentEmployer,
    })
  | NOT_EMPLOYED =>
    Explain(
      switch (explain) {
      | Some(expl) => expl
      | None => raise(Failure("Please explain if you are not employed"))
      },
    )
  | _ => Employer(presentEmployer)
  };
};

let pluckPreviousEmployerName = makePluckerMulti("Previous_Employer_Name");

let pluckPreviousEmployerAddress1 =
  makePluckerMulti("Previous_Employer_ADDR1");
let pluckPreviousEmployerAddress2 =
  makePluckerMulti("Previous_Employer_ADDR2");
let pluckPreviousEmployerAddressCity =
  makePluckerMulti("Previous_Employer_ADDR_City");
let pluckPreviousEmployerAddressState =
  makePluckerMulti("Previous_Employer_ADDR_State");
let pluckPreviousEmployerAddressPostalCode =
  makePluckerMulti("Previous_Employer_ADDR_Postal_CD");
let pluckPreviousEmployerAddressCountry =
  makePluckerMulti("Previous_Employer_ADDR_Country");

let pluckPreviousEmployerPhoneNum = makePluckerMulti("Previous_Employer_TEL");
let pluckPreviousEmployerJob_Title =
  makePluckerMulti("Previous_Employer_Job_Title");
let pluckPreviousEmployerSupervisorSurname =
  makePluckerMulti("Previous_Employer_Supervisor_Surname");
let pluckPreviousEmployerSupervisorGiveName =
  makePluckerMulti("Previous_Employer_Supervisor_Given_Name");

let pluckPreviousEmployerStartDay =
  makePluckerMulti("Previous_Employer_Start_Day");
let pluckPreviousEmployerStartMonth =
  makePluckerMulti("Previous_Employer_Start_Month");
let pluckPreviousEmployerStartYear =
  makePluckerMulti("Previous_Employer_Start_Year");
let pluckPreviousEmployerEndDay =
  makePluckerMulti("Previous_Employer_End_Day");
let pluckPreviousEmployerEndMonth =
  makePluckerMulti("Previous_Employer_End_Month");
let pluckPreviousEmployerEndYear =
  makePluckerMulti("Previous_Employer_End_Year");
let pluckPreviousEmployerDuty = makePluckerMulti("Previous_Employer_Duty");

let pluckPreviousEmployer =
    (
      employerName,
      streetL1,
      streetL2,
      city,
      state,
      zipCode,
      country,
      phoneNum,
      jobTitle,
      supervisorSurname,
      supervisorGivenName,
      startDay,
      startMonth,
      startYear,
      endDay,
      endMonth,
      endYear,
      duty,
    ) =>
  switch (
    employerName,
    streetL1,
    streetL2,
    city,
    state,
    zipCode,
    country,
    phoneNum,
    jobTitle,
    supervisorSurname,
    supervisorGivenName,
    startDay,
    startMonth,
    startYear,
    endDay,
    endMonth,
    endYear,
    duty,
  ) {
  | (
      None,
      None,
      None,
      None,
      None,
      None,
      None,
      None,
      None,
      None,
      None,
      None,
      None,
      None,
      None,
      None,
      None,
      None,
    ) =>
    None
  | (
      Some(employerName),
      Some(streetL1),
      Some(streetL2),
      Some(city),
      Some(state),
      Some(zipCode),
      Some(country),
      Some(phoneNum),
      Some(jobTitle),
      Some(supervisorSurname),
      Some(supervisorGivenName),
      Some(startDay),
      Some(startMonth),
      Some(startYear),
      Some(endDay),
      Some(endMonth),
      Some(endYear),
      Some(duty),
    ) =>
    let employerAddress: list(DsDataTypes.address) =
      listAddress(streetL1, streetL2, city, state, zipCode, country);
    let startDate = pluckListDate(startDay, startMonth, startYear);
    let endDate = pluckListDate(endDay, endMonth, endYear);
    let listLength = List.length(employerName);
    if (listLength == List.length(employerName)
        && listLength == List.length(employerAddress)
        && listLength == List.length(phoneNum)
        && listLength == List.length(jobTitle)
        && listLength == List.length(supervisorSurname)
        && listLength == List.length(supervisorGivenName)
        && listLength == List.length(startDate)
        && listLength == List.length(endDate)
        && listLength == List.length(duty)) {
      Some(
        List.mapi(
          (idx, employerName) => (
            {
              employerName,
              employerAddress: List.nth(employerAddress, idx),
              phoneNum: List.nth(phoneNum, idx),
              jobTitle: List.nth(jobTitle, idx),
              supervisorSurname:
                switch (List.nth(supervisorSurname, idx)) {
                | "N/A" => None
                | _name => Some(_name)
                },
              supervisorGivenName:
                switch (List.nth(supervisorGivenName, idx)) {
                | "N/A" => None
                | _name => Some(_name)
                },
              startDate: List.nth(startDate, idx),
              endDate: List.nth(endDate, idx),
              duty: List.nth(duty, idx),
            }: DsDataTypes.previousEmployer
          ),
          employerName,
        ),
      );
    } else {
      raise(
        Failure(
          "The Following Fields must be in same length:
       employerName,
       employerAddress,
       phoneNum,
       jobTitle,
       supervisorSurname,
       supervisorGivenName,
       startDate,
       endDate,
       duty. Please enter N/A for unknown fields",
        ),
      );
    };
  | _ =>
    raise(
      Failure(
        "The Following Fields must be in same length:
       employerName,
       employerAddress,
       phoneNum,
       jobTitle,
       supervisorSurname,
       supervisorGivenName,
       startDate,
       endDate,
       duty. Please enter N/A for unknown fields",
      ),
    )
  };

let pluckPreviousSchoolName = makePluckerMulti("Previous_School_Name");

let pluckPreviousSchoolAddress1 = makePluckerMulti("Previous_School_ADDR1");
let pluckPreviousSchoolAddress2 = makePluckerMulti("Previous_School_ADDR2");
let pluckPreviousSchoolAddressCity =
  makePluckerMulti("Previous_School_ADDR_City");
let pluckPreviousSchoolAddressState =
  makePluckerMulti("Previous_School_ADDR_State");
let pluckPreviousSchoolAddressPostalCode =
  makePluckerMulti("Previous_School_ADDR_Postal_CD");
let pluckPreviousSchoolAddressCountry =
  makePluckerMulti("Previous_School_ADDR_Country");

let pluckPreviousSchoolPhoneNum = makePluckerMulti("Previous_School_TEL");
let pluckPreviousSchoolCourseOfStudy =
  makePluckerMulti("Previous_School_Course_Of_Study");
let pluckPreviousSchoolStartDay =
  makePluckerMulti("Previous_School_Start_Day");
let pluckPreviousSchoolStartMonth =
  makePluckerMulti("Previous_School_Start_Month");
let pluckPreviousSchoolStartYear =
  makePluckerMulti("Previous_School_Start_Year");
let pluckPreviousSchoolEndDay = makePluckerMulti("Previous_School_End_Day");
let pluckPreviousSchoolEndMonth =
  makePluckerMulti("Previous_School_End_Month");
let pluckPreviousSchoolEndYear = makePluckerMulti("Previous_School_End_Year");

let pluckPreviousEducation =
    (
      schoolName,
      streetL1,
      streetL2,
      city,
      state,
      zipCode,
      country,
      phoneNum,
      courseOfStudy,
      startDay,
      startMonth,
      startYear,
      endDay,
      endMonth,
      endYear,
    ) =>
  switch (
    schoolName,
    streetL1,
    streetL2,
    city,
    state,
    zipCode,
    country,
    phoneNum,
    courseOfStudy,
    startDay,
    startMonth,
    startYear,
    endDay,
    endMonth,
    endYear,
  ) {
  | (
      None,
      None,
      None,
      None,
      None,
      None,
      None,
      None,
      None,
      None,
      None,
      None,
      None,
      None,
      None,
    ) =>
    None
  | (
      Some(schoolName),
      Some(streetL1),
      Some(streetL2),
      Some(city),
      Some(state),
      Some(zipCode),
      Some(country),
      Some(phoneNum),
      Some(courseOfStudy),
      Some(startDay),
      Some(startMonth),
      Some(startYear),
      Some(endDay),
      Some(endMonth),
      Some(endYear),
    ) =>
    let employerAddress: list(DsDataTypes.address) =
      listAddress(streetL1, streetL2, city, state, zipCode, country);
    let startDate = pluckListDate(startDay, startMonth, startYear);
    let endDate = pluckListDate(endDay, endMonth, endYear);
    let listLength = List.length(schoolName);
    if (listLength == List.length(schoolName)
        && listLength == List.length(employerAddress)
        && listLength == List.length(phoneNum)
        && listLength == List.length(courseOfStudy)
        && listLength == List.length(startDate)
        && listLength == List.length(endDate)) {
      Some(
        List.mapi(
          (idx, institutionName) => (
            {
              institutionName,
              employerAddress: List.nth(employerAddress, idx),
              phoneNum: List.nth(phoneNum, idx),
              courseOfStudy: List.nth(courseOfStudy, idx),
              startDate: List.nth(startDate, idx),
              endDate: List.nth(endDate, idx),
            }: DsDataTypes.education
          ),
          schoolName,
        ),
      );
    } else {
      raise(
        Failure(
          "The Following Fields must be in same length:
           employerName,
           employerAddress,
           phoneNum,
           jobTitle,
           supervisorSurname,
           supervisorGivenName,
           startDate,
           endDate,
           duty. Please enter N/A for unknown fields",
        ),
      );
    };
  | _ =>
    raise(
      Failure(
        "The Following Fields must be in same length:
         employerName,
         employerAddress,
         phoneNum,
         jobTitle,
         supervisorSurname,
         supervisorGivenName,
         startDate,
         endDate,
         duty. Please enter N/A for unknown fields",
      ),
    )
  };
