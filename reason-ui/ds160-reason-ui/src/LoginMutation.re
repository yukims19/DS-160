[@bs.module] external gql: ReasonApolloTypes.gql = "graphql-tag";
module Login = [%graphql
  {|
     mutation($username: String!, $password:String!){
     login(username: $username, password: $password)
     }
|}
];

module LoginMutation = ReasonApollo.CreateMutation(Login);

let component = ReasonReact.statelessComponent("LoginMutaiton");

let make = (~username, ~password, _children) => {
  ...component,
  render: _self =>
    <LoginMutation>
      ...{
           (mutation, _) => {
             let loginQuery = Login.make(~username, ~password, ());
             <button
               className="btn btn-primary"
               onClick={
                 _e =>
                   if (String.length(username) > 0
                       && String.length(password) > 0) {
                     mutation(~variables=loginQuery##variables, ()) |> ignore;
                   } else {
                     %bs.raw
                     {| alert('Please Enter Username and Password!') |};
                   }
               }>
               {ReasonReact.string("Login")}
             </button>;
           }
         }
    </LoginMutation>,
};
