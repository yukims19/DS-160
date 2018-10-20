/* Country and Occupation data type????*/
open CountryType;

type name = {
  surname: string,
  givenName: string,
};
type sex =
  | Male
  | Female;

type marital =
  | Married
  | Single
  | Common
  | Civil
  | Widowed
  | Divorced
  | Seperate
  | Other;

type month =
  | Jan
  | Feb
  | Mar
  | Apr
  | May
  | Jun
  | Jul
  | Aug
  | Sep
  | Oct
  | Nov
  | Dec;

type date = {
  day: string,
  month,
  year: string,
};
type shortAddress = {
  city: string,
  state: option(string),
  country,
};

type personal1 = {
  name,
  nativeFullName: option(string),
  otherNames: option(list(name)),
  telecodeName: option(name),
  sex,
  maritalStatus: marital,
  dateOfBirth: date,
  placeOfBirth: shortAddress,
};

/*-----------------Page2----------------*/
type nationalityInfo = {
  nationality: country,
  passportNum: option(string),
};

type personal2 = {
  nationality: country,
  otherNationality: option(list(nationalityInfo)),
  permanentResident: option(list(country)),
  nationalId: option(string),
  ssn: option(string),
  taxId: option(string),
};
/*-----------------Page3----------------*/
type address = {
  streetL1: string,
  streetL2: option(string),
  city: string,
  state: option(string),
  zipCode: option(string),
  country,
};
type addressAndPhone = {
  homeAddress: address,
  mailingAddress: option(address),
  primaryPhone: string,
  secPhone: option(string),
  workPhone: option(string),
  email: string,
};
/*-----------------Page4----------------*/
type passType =
  | Regular
  | Official
  | Diplomatic
  | Laissez
  | Other;

type lostPassInfo = {
  passNum: option(string),
  countryIssue: country,
  explain: string,
};

type passport = {
  passType,
  passNum: string,
  passBookNum: option(string),
  countryIssue: string,
  placeIssue: shortAddress,
  issueDate: date,
  expirationDate: option(date),
  lostPass: option(list(lostPassInfo)),
};
/*-----------------Page5----------------*/
type purpose =
  | A
  | B
  | C
  | D
  | E
  | F
  | G;

type hasTravelPlan = {
  arrivalDate: date,
  arrivalFlight: option(string),
  arrivalCity: string,
  departureDate: date,
  departureFlight: option(string),
  departureCity: string,
  visitLocation: list(string),
  stayAddress: address,
};

type timeUnit =
  | Y
  | M
  | W
  | D;

type timeLength = {
  number: int,
  timeUnit,
};

type noTravelPlan = {
  arrivalDate: date,
  stayLength: timeLength,
  stayAddress: address,
};

type travelPlan =
  | HasTravelPlan(hasTravelPlan)
  | NoTravelPlan(noTravelPlan);

type purposeInfo = {
  purpose,
  specify: string,
};
type personPaying =
  | Self
  | OtherPerson
  | Employee
  | USEmployee
  | OtherOrg;
type travel = {
  purposeToUS: list(purposeInfo),
  travelPlan,
  personPaying,
};

/*-----------------Page6----------------*/
type relationship =
  | Parent
  | Spouse
  | Child
  | Relative
  | Friend
  | Bussiness
  | Other;
/*********/
type org = string;
type personAndRelationship = {
  name,
  relationship,
};
type companion =
  | Org(org)
  | PersonAndRelationship(personAndRelationship);
/*********/
type travelCompanions = {companion: option(companion)};

/*-----------------Page7----------------*/
type preTravelInfo = {
  arrivalDate: date,
  timeLength,
};
type driverLicenseInfo = {
  licenseNum: option(string),
  state: string,
};
type lostVisaInfo = {
  year: string,
  explain: string,
};
type visaInfo = {
  issueDate: date,
  visaNum: option(string),
  sameVisa: bool,
  sameLocation: bool,
  tenPrint: bool,
  lost: option(lostVisaInfo),
  cancel: option(string),
};

type previousUSTravel = {
  lastTravels: option(list(preTravelInfo)),
  hasDriverLicense: option(list(driverLicenseInfo)),
  preVisa: option(visaInfo),
  refused: option(string),
  petition: option(string),
};
/*-----------------Page8----------------*/
type person = name;
type contactPersonOrOrg =
  | Person(person)
  | Org(org);
type contactRelationship =
  | Relative
  | Spouse
  | Friend
  | Bussiness
  | Employer
  | School
  | Other;
type usContact = {
  contactPersonOrOrg,
  contactRelationship,
  address,
  phoneNum: string,
  email: option(string),
};
/*-----------------Page9----------------*/
type stayStatus =
  | Citizen
  | LPR
  | NonImmigrant
  | Other;
type familyMember = {
  surname: option(string),
  givenName: option(string),
  dateOfBirth: option(date),
  stayStatus: option(stayStatus),
};

type relativeRelationship =
  | Sibling
  | Spouse
  | Child
  | Fiance;
type relativeMember = {
  name,
  relativeRelationship,
  stayStatus,
};
type family = {
  father: familyMember,
  mother: familyMember,
  relative: option(list(relativeMember)),
};
/*-----------------Page10----------------*/
type explain = string;
type presentEmployer = {
  employerName: string,
  employerAddress: address,
  phoneNum: string,
  startDate: date,
  monthlyIncome: option(int),
  duty: string,
};
type explainAndEmployer = {
  explain,
  presentEmployer,
};
type presentEmployerInfo =
  | Explain(explain)
  | Employer(presentEmployer)
  | ExplainAndEmployer(explainAndEmployer);

type presentWET = {
  occupation: OccupationType.occupation, /*TODO: Need to be variant
     Other => explain and employer
     No Employ => explain
     _ => employer
   */
  presentEmployerInfo,
};
/*-----------------Page11----------------*/
type previousEmployer = {
  employerName: string,
  employerAddress: address,
  phoneNum: string,
  jobTitle: string,
  supervisorSurname: option(string),
  supervisorGivenName: option(string),
  startDate: date,
  endDate: date,
  duty: string,
};
type education = {
  institutionName: string,
  employerAddress: address,
  phoneNum: string,
  courseOfStudy: string,
  startDate: date,
  endDate: date,
};

type previousWET = {
  previousEmployer: option(list(previousEmployer)),
  education: option(list(education)),
};
/*-----------------Page12----------------*/
type military = {
  country,
  branch: string,
  rank: string,
  speciality: string,
  startDate: date,
  endDate: date,
};
type additionalWET = {
  tribe: option(string),
  language: list(string),
  travel: option(list(country)),
  charity: option(list(string)),
  specialSkill: option(string),
  military: option(list(military)),
  rebel: option(string),
};
/*****************E_VISA***************************/
type applicantType =
  | Owner
  | Supervisor
  | Specialist
  | Manager
  | Executive
  | Other;
type applicantPresentPositionEVisa = {
  applicantType,
  duty: string,
  employerName: string,
  years: int,
  address,
  school: string,
  degree: string,
  major: string,
  schoolYear: string,
  otherEducation: option(string),
};
/*--------------------*/
type applicantPositionInUS = {
  title: string,
  duty: string,
  salary: int,
  allowance: int,
  total: int,
};
/*--------------------*/
type contactPerson = {
  name,
  address,
  phoneNum: string,
  faxNum: option(string),
  email: option(string),
};
type applicantContactInfo = {
  officerSurname: string,
  officerGiveName: string,
  officerPosition: string,
  contactPerson,
};

/*****************Temp_Work_Visa_Info***************************/
type tempWorkVisaInfo = {
  employerName: string,
  companyRegistrationNum: option(string),
  address,
  phoneNum: string,
};

/*****************String of Data***************************/
let stringOfGender =
  fun
  | Female => "F"
  | Male => "M";

let stringOfMartialStatus =
  fun
  | Married => "M"
  | Single => "S"
  | Common => "C"
  | Civil => "P"
  | Widowed => "W"
  | Divorced => "D"
  | Seperate => "L"
  | Other => "O";

let stringOfMonth =
  fun
  | Jan => "Jan"
  | Feb => "Feb"
  | Mar => "Mar"
  | Apr => "Apr"
  | May => "May"
  | Jun => "Jun"
  | Jul => "Jul"
  | Aug => "Aug"
  | Sep => "Sep"
  | Oct => "Oct"
  | Nov => "Nov"
  | Dec => "Dec";

let stringOfPassportType =
  fun
  | Regular => "Regular"
  | Official => "Official"
  | Diplomatic => "Diplomatic"
  | Laissez => "Laissez"
  | Other => "Other";

let stringOfDate = date =>
  Printf.sprintf(
    "%s %s, %s",
    stringOfMonth(date.month),
    date.day,
    date.year,
  );
let stringOfOptionDate = date =>
  switch (date) {
  | Some(date) =>
    Printf.sprintf(
      "%s %s, %s",
      stringOfMonth(date.month),
      date.day,
      date.year,
    )
  | None => "No Date Found"
  };

let stringOfOptionString = (key, value) =>
  switch (value) {
  | Some(value) => value
  | None => "No value found for " ++ key
  };

let stringOfOptionListString = (key, value) =>
  switch (value) {
  | Some(value) => value |> List.fold_left((a, b) => a ++ "/" ++ b, "")
  | None => "No value found for " ++ key
  };
let stringOfOptionListNationalityInfo = otherNationality =>
  switch (otherNationality) {
  | Some(listOfNationalityInfo) =>
    String.concat(
      "/",
      List.map(
        nationalityInfo =>
          switch (nationalityInfo.passportNum) {
          | Some(num) =>
            "Nation: "
            ++ stringOfCountry(nationalityInfo.nationality)
            ++ "Passport Number: "
            ++ num
          | None =>
            "Nation: "
            ++ stringOfCountry(nationalityInfo.nationality)
            ++ "Passport Number Unknown"
          },
        listOfNationalityInfo,
      ),
    )
  | None => "No Other Nationality"
  };

let stringOfOptionListCountry = countries =>
  switch (countries) {
  | Some(countries) =>
    String.concat(
      "/",
      List.map(country => stringOfCountry(country), countries),
    )
  | None => "No permanent resident information found"
  };
let stringOfAddress = address => {
  let streetL1 = address.streetL1;
  let streetL2 =
    switch (address.streetL2) {
    | Some(street) => street
    | None => "N/A"
    };
  let city = address.city;
  let state =
    switch (address.state) {
    | Some(state) => state
    | None => "N/A"
    };
  let zipCode =
    switch (address.zipCode) {
    | Some(code) => code
    | None => "N/A"
    };
  let country = CountryType.stringOfCountry(address.country);
  Printf.sprintf(
    "
     Street1:%s,
     Street2: %s,
     City: %s,
     State; %s,
     ZipCode: %s,
     Country: %s
     ",
    streetL1,
    streetL2,
    city,
    state,
    zipCode,
    country,
  );
};
let stringOfOptionAddress = address =>
  switch (address) {
  | Some(address) => stringOfAddress(address)
  | None => "No Address Found"
  };

let stringOfShortAddress = (shortAddress: shortAddress) => {
  let city = shortAddress.city;
  let state =
    switch (shortAddress.state) {
    | Some(state) => state
    | None => "N/A"
    };
  let country = CountryType.stringOfCountry(shortAddress.country);
  Printf.sprintf(
    "
       City: %s,
       State; %s,
       Country: %s
       ",
    city,
    state,
    country,
  );
};

let stringOfLostPassport = (lostPass: option(list(lostPassInfo))) =>
  switch (lostPass) {
  | Some(listOfLostPass) =>
    String.concat(
      "/",
      List.map(
        (lostPass: lostPassInfo) =>
          switch (lostPass.passNum) {
          | Some(num) =>
            "Nation: "
            ++ stringOfCountry(lostPass.countryIssue)
            ++ "Passport Number: "
            ++ num
            ++ "Explain: "
            ++ lostPass.explain
          | None =>
            "Nation: "
            ++ stringOfCountry(lostPass.countryIssue)
            ++ "Passport Number Unknown"
            ++ "Explain: "
            ++ lostPass.explain
          },
        listOfLostPass,
      ),
    )
  | None => "No Other Nationality"
  };

let stringOfPersonPaying = personPayingType =>
  switch (personPayingType) {
  | Self => "Self"
  | OtherPerson => "OtherPerson"
  | Employee => "Employee"
  | USEmployee => "USEmployee"
  | OtherOrg => "OtherOrg"
  };

let stringOfTimeUnit = timeUnit =>
  switch (timeUnit) {
  | Y => "Y"
  | M => "M"
  | W => "W"
  | D => "D"
  };
let stringOfTimeLength = timeLength =>
  string_of_int(timeLength.number) ++ stringOfTimeUnit(timeLength.timeUnit);

let stringOfTravelPlan = travelPlan =>
  switch (travelPlan) {
  | NoTravelPlan(noTravelPlan) =>
    Printf.sprintf(
      "
       Arrival Date: %s,
       Stay Length: %s,
       Stay Address: %s",
      stringOfDate(noTravelPlan.arrivalDate),
      stringOfTimeLength(noTravelPlan.stayLength),
      stringOfAddress(noTravelPlan.stayAddress),
    )
  | HasTravelPlan(hasTravelPlan) => ""
  };

let stringOfPurpose = purpose =>
  switch (purpose) {
  | A => "A"
  | B => "B"
  | C => "C"
  | D => "D"
  | E => "E"
  | F => "F"
  | G => "G"
  };
let stringOfPurposeToUS = purposeInfo => {
  let purposeList =
    List.map(
      purposeInfo =>
        stringOfPurpose(purposeInfo.purpose) ++ "-" ++ purposeInfo.specify,
      purposeInfo,
    );
  List.fold_left((a, b) => a ++ "/" ++ b, "", purposeList);
};

let stringOfRelationship = relationship =>
  switch (relationship) {
  | Parent => "Parent"
  | Spouse => "Spouse"
  | Child => "Child"
  | Relative => "Relative"
  | Friend => "Friend"
  | Bussiness => "Bussiness"
  | Other => "Other"
  };

let stringOfPersonAndRelationship =
    (personAndRelationship: personAndRelationship) =>
  Printf.sprintf(
    "
     Name: %s,
     Relationship: %s",
    personAndRelationship.name.surname ++ personAndRelationship.name.givenName,
    stringOfRelationship(personAndRelationship.relationship),
  );

let stringOfTravelCompanion = (companion: option(companion)) =>
  switch (companion) {
  | None => "N/A"
  | Some(companion) =>
    switch (companion) {
    | Org(org) => "With organization: " ++ org
    | PersonAndRelationship(personAndRelationship) =>
      "With person" ++ stringOfPersonAndRelationship(personAndRelationship)
    }
  };

let stringOfPreTravelInfo = optionListPreTravelInfo =>
  switch (optionListPreTravelInfo) {
  | None => "N/A"
  | Some(listPreTravelInfo) =>
    List.mapi(
      (idx, preTravelInfo) =>
        Printf.sprintf(
          "
           %s:
           Arrival Date: %s,
           Stay Length: %s",
          string_of_int(idx),
          stringOfDate(preTravelInfo.arrivalDate),
          stringOfTimeLength(preTravelInfo.timeLength),
        ),
      listPreTravelInfo,
    )
    |> List.fold_left((a, b) => a ++ "/" ++ b, "")
  };

let stringOfDriverLicense = optionListDriverLicenseInfo =>
  switch (optionListDriverLicenseInfo) {
  | None => "N/A"
  | Some(listDriverLicenseInfo) =>
    List.mapi(
      (idx, driverLicenseInfo) =>
        Printf.sprintf(
          "
         %s:
         License Number: %s,
         State: %s",
          string_of_int(idx),
          switch (driverLicenseInfo.licenseNum) {
          | None => "N/A"
          | Some(num) => num
          },
          driverLicenseInfo.state,
        ),
      listDriverLicenseInfo,
    )
    |> List.fold_left((a, b) => a ++ "/" ++ b, "")
  };

let stringOfLostVisaInfo = lostVisaInfo =>
  Printf.sprintf(
    "Lost Year: %s,
     Explain: %s",
    lostVisaInfo.year,
    lostVisaInfo.explain,
  );
let stringOfPreVisa = optionVisaInfo =>
  switch (optionVisaInfo) {
  | None => "N/A"
  | Some(visaInfo) =>
    Printf.sprintf(
      "Issue Date: %s,
       Visa Number: %s,
       Is Same Visa Type: %s,
       Is Same Visa Location: %s,
       Has Ten Print: %s,
       Lost Explain: %s,
       Canceled Explain: %s
       ",
      stringOfDate(visaInfo.issueDate),
      switch (visaInfo.visaNum) {
      | None => "N/A"
      | Some(num) => num
      },
      string_of_bool(visaInfo.sameVisa),
      string_of_bool(visaInfo.sameLocation),
      string_of_bool(visaInfo.tenPrint),
      switch (visaInfo.lost) {
      | None => "N/A"
      | Some(lostVisaInfo) => stringOfLostVisaInfo(lostVisaInfo)
      },
      switch (visaInfo.cancel) {
      | None => "N/A"
      | Some(exple) => exple
      },
    )
  };

let stringOfContactPersonOrOrg = contactPersonOrOrg =>
  switch (contactPersonOrOrg) {
  | Org(org) => org
  | Person(person) => person.surname ++ ", " ++ person.givenName
  };

let stringOfContactRelationship = contactRelationship =>
  switch (contactRelationship) {
  | Relative => "Relative"
  | Spouse => "Spouse"
  | Friend => "Friend"
  | Bussiness => "Bussiness"
  | Employer => "Employer"
  | School => "School"
  | Other => "Other"
  };

let stringOfStayStatus = stayStatus =>
  switch (stayStatus) {
  | Citizen => "Citizen"
  | LPR => "LPR"
  | NonImmigrant => "NonImmigrant"
  | Other => "Other"
  };

let stringOfFamilyMember = familyMember =>
  Printf.sprintf(
    "
     %s
     %s
     %s
     %s
     ",
    stringOfOptionString("Surname", familyMember.surname),
    stringOfOptionString("GivenName", familyMember.givenName),
    switch (familyMember.dateOfBirth) {
    | None => "UnKnown"
    | Some(date) => stringOfDate(date)
    },
    switch (familyMember.stayStatus) {
    | None => "N/A"
    | Some(status) => stringOfStayStatus(status)
    },
  );

let stringOfRelativeRelationship = relationship =>
  switch (relationship) {
  | Sibling => "Sibling"
  | Spouse => "Spouse"
  | Child => "Child"
  | Fiance => "Fiance"
  };
let stringOfRelativeMember =
    (optionListRelativeMember: option(list(relativeMember))) =>
  switch (optionListRelativeMember) {
  | None => "N/A"
  | Some(listRelativeMember) =>
    List.map(
      (relativeMember: relativeMember) =>
        Printf.sprintf(
          "
           %s,
           %s,
           %s,
           ",
          relativeMember.name.surname ++ ", " ++ relativeMember.name.givenName,
          stringOfRelativeRelationship(relativeMember.relativeRelationship),
          stringOfStayStatus(relativeMember.stayStatus),
        ),
      listRelativeMember,
    )
    |> List.fold_left((a, b) => a ++ "/" ++ b, "")
  };

let stringOfPresentEmployer = (presentEmployer: presentEmployer) =>
  Printf.sprintf(
    "
     Employer Name: %s,
     EmployerAddress: %s,
     Phone Number: %s,
     Start Date: %s,
     Monthly Income: %s,
     Duty: %s
     ",
    presentEmployer.employerName,
    stringOfAddress(presentEmployer.employerAddress),
    presentEmployer.phoneNum,
    stringOfDate(presentEmployer.startDate),
    switch (presentEmployer.monthlyIncome) {
    | None => "N/A"
    | Some(income) => string_of_int(income)
    },
    presentEmployer.duty,
  );
let stringOfPresentEmployerInfo = presentEmployerInfo =>
  switch (presentEmployerInfo) {
  | Explain(explain) => explain
  | Employer(presentEmployer) => stringOfPresentEmployer(presentEmployer)
  | ExplainAndEmployer(explainAndEmployer) =>
    "Explain: "
    ++ explainAndEmployer.explain
    ++ stringOfPresentEmployer(explainAndEmployer.presentEmployer)
  };

let stringOfPreviousEmployer =
    (previousEmployers: option(list(previousEmployer))) =>
  switch (previousEmployers) {
  | None => "N/A"
  | Some(previousEmployers) =>
    List.map(
      (previousEmployer: previousEmployer) =>
        Printf.sprintf(
          "
         Employer Name: %s,
         EmployerAddress: %s,
         Phone Number: %s,
         Job Title: %s,
         Supervisor Surname: %s,
         Supervisor Given Name: %s,
         Start Date: %s,
         End Date: %s,
         Duty: %s
         ",
          previousEmployer.employerName,
          stringOfAddress(previousEmployer.employerAddress),
          previousEmployer.phoneNum,
          previousEmployer.jobTitle,
          switch (previousEmployer.supervisorSurname) {
          | Some(name) => name
          | None => "N/A"
          },
          switch (previousEmployer.supervisorGivenName) {
          | Some(name) => name
          | None => "N/A"
          },
          stringOfDate(previousEmployer.startDate),
          stringOfDate(previousEmployer.endDate),
          previousEmployer.duty,
        ),
      previousEmployers,
    )
    |> List.fold_left((a, b) => a ++ "/" ++ b, "")
  };

let stringOfPreviousEducation =
    (previousEducations: option(list(education))) =>
  switch (previousEducations) {
  | None => "N/A"
  | Some(previousEducations) =>
    List.map(
      (previousEducation: education) =>
        Printf.sprintf(
          "
         Institution Name: %s,
         Institution Address: %s,
         Phone Number: %s,
         Course Of Study: %s,
         Start Date: %s,
         End Date: %s,
         ",
          previousEducation.institutionName,
          stringOfAddress(previousEducation.employerAddress),
          previousEducation.phoneNum,
          previousEducation.courseOfStudy,
          stringOfDate(previousEducation.startDate),
          stringOfDate(previousEducation.endDate),
        ),
      previousEducations,
    )
    |> List.fold_left((a, b) => a ++ "/" ++ b, "")
  };
