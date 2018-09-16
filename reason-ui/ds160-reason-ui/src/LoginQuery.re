[@bs.module] external gql: ReasonApolloTypes.gql = "graphql-tag";
module GetUserLogin = [%graphql
  {|
     query($username: String!, $password:String!){
     users(username: $username, password: $password)
     }
|}
];

module GetUserLoginQuery = ReasonApollo.CreateQuery(GetUserLogin);

let component = ReasonReact.statelessComponent("LoginQuery");

let make = (~checkLogin, _children) => {
  ...component,
  render: _self => {
    let loginQuery = GetUserLogin.make(~username="yuki", ~password="123", ());
    <div>
      <GetUserLoginQuery variables=loginQuery##variables>
        ...{
             ({result}) =>
               switch (result) {
               | Loading => <div> {"Loading" |> ReasonReact.string} </div>
               | Error(e) => <div> {ReasonReact.string(e##message)} </div>
               | Data(response) =>
                 switch (response##users) {
                 | Some(status) =>
                   checkLogin(status);
                   if (status) {
                     print_endline("Logged in");
                   } else {
                     print_endline("Failed to login");
                   };
                   <div> {ReasonReact.string(string_of_bool(status))} </div>;
                 | None => <div />
                 }
               }
           }
      </GetUserLoginQuery>
    </div>;
  },
};
