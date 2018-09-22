module FindMe = [%graphql
  {|
     query findMe {
      me {
       username
       id
     }
     }
|}
];

module FindMeQuery = ReasonApollo.CreateQuery(FindMe);

let component = ReasonReact.statelessComponent("FindMe");

open ReasonReact;

let make = children => {
  ...component,
  render: _self =>
    <div>
      <FindMeQuery>
        ...{
             ({result}) =>
               switch (result) {
               | Loading => <div> {string("Loading LoginComponent")} </div>
               | Error(e) => <div> {string(e##message)} </div>
               | Data(response) =>
                 switch (response##me) {
                 | Some(_user) => array(children)
                 | None => <LoginForm />
                 }
               }
           }
      </FindMeQuery>
    </div>,
};
