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
  country: string,
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
  | Result
  | Official
  | Diplomatic
  | Laissez
  | Other;

type lostPassInfo = {
  passNum: option(string),
  countryIssue: string,
  explain: string,
};

type passport = {
  passType,
  passNum: string,
  passBookNum: option(string),
  countryIssue: string,
  placeIssue: shortAddress,
  isscueDate: date,
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

type hasTravlePlan = {
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

type noTravlePlan = {
  arrivalDate: date,
  stayLength: timeLength,
  stayAddress: address,
};

type travelPlan =
  | HasTravlePlan
  | NoTravlePlan;

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
  | Org
  | PersonAndRelationship;
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
  | Person
  | Org;
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
  surnames: option(string),
  givenName: option(string),
  dateOfBirth: option(date),
  usStatus: option(stayStatus),
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
type presentEmployerInfo =
  | Explain
  | Employer
  | ExplainAndEmployer;
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
type presentWET = {
  occupation: string,
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
  country: string,
  branch: string,
  rank: string,
  speciality: string,
  startDate: date,
  endDate: date,
};
type additionalWET = {
  tribe: option(string),
  lanuage: list(string),
  travle: option(list(string)),
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

let stringOfDate = date =>
  Printf.sprintf(
    "%s %s, %s",
    stringOfMonth(date.month),
    date.day,
    date.year,
  );

let stringOfFullName = fullName =>
  switch (fullName) {
  | Some(name) => name
  | None => "No native full name found"
  };
