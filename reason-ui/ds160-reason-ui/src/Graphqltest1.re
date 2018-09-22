[@bs.module] external gql: ReasonApolloTypes.gql = "graphql-tag";
module GetTopStories = [%graphql {|
     query{
     greeter
     }
|}];

module GetTopStroriesQuery = ReasonApollo.CreateQuery(GetTopStories);

let component = ReasonReact.statelessComponent("Graphqltest1");

let make = _children => {
  ...component,
  render: _self =>
    <div>
      <GetTopStroriesQuery>
        ...{
             ({result}) =>
               switch (result) {
               | Loading => <div> {"Loading" |> ReasonReact.string} </div>
               | Error(e) => <div> {ReasonReact.string(e##message)} </div>
               | Data(response) =>
                 <h1> {ReasonReact.string(response##greeter)} </h1>
               }
           }
      </GetTopStroriesQuery>
    </div>,
};
