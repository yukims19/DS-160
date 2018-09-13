open Async;
open Graphql_async;

/**
   TODO:
   1. Add pet type
   `Pet` should have 10 fields, some optional (nullable), some required:
   1a. Integer
   1b. String
   1c. Enum
   1d. Other

   2. Add custom enum type ("CAT" or "DOG" or "BIRD") - Enum is GraphQL's variant (but less powerful)
   3. Add relationship type (A user can have multiple pets, and a pet can have an "owner" field)
   4. Add arguments to some fields
   5. Add a "top story on hacker news" io field (do this last)
 */

let baseHnUri = "https://hacker-news.firebaseio.com";

let get = (~path: string) => {
  let uriString = Printf.sprintf("%s%s", baseHnUri, path);
  let uri = Uri.of_string(uriString);
  let headers =
    Cohttp.Header.of_list([
      ("User-Agent", "DS160"),
      ("Accept", "application/json"),
      ("content-type", "application/json"),
    ]);
  Cohttp_async.Client.get(~headers, uri)
  >>= (
    ((_resp, body)) =>
      body
      |> Cohttp_async.Body.to_string
      >>| (
        body => {
          OneLog.infof("Return from website:\n%s", body);
          try (Ok(Yojson.Basic.from_string(body))) {
          | ex => Error(ex)
          };
        }
      )
  );
};

let getJsonValueString = (key: string, json) =>
  Yojson.Basic.Util.to_string(Yojson.Basic.Util.member(key, json));
let getJsonValueInt = (key: string, json) =>
  Yojson.Basic.Util.to_int(Yojson.Basic.Util.member(key, json));
let getJsonString = json => Yojson.Basic.to_string(json);
let getJsonInt = json => Yojson.Basic.Util.to_int(json);

type role =
  | User
  | Admin;

type species =
  | Cat
  | Dog
  | Bird
  | Others;

type pet = {
  id: int,
  name: string,
  color: string,
  species,
  owner: option(user),
}
and user = {
  id: int,
  name: string,
  role,
  friends: list(user),
  pets: list(pet),
};

type story = {
  id: int,
  title: string,
  url: string,
};

/* here to just define dummy hackerNewsData*/
type hackernews = {topStory: Deferred.t(list(story))};

let topStoryIdPath = "/v0/topstories.json?print=pretty";
let getTopStoryList = () => {
  let topStoryIdResult = get(topStoryIdPath);
  let topStoryIdList =
    topStoryIdResult
    >>= (
      json =>
        switch (json) {
        | Error(ex) => raise(ex)
        | Ok(json) =>
          let step1 = json |> Yojson.Basic.Util.to_list;
          let step2 = List.map(Yojson.Basic.Util.to_int, step1);
          Deferred.return(step2);
        }
    );
  let topStoryList =
    topStoryIdList
    >>= (
      storyIds => {
        let subStoryIds = Core.List.sub(storyIds, 0, 10);
        subStoryIds
        |> List.map(id => {
             let path = "/v0/item/" ++ string_of_int(id) ++ ".json";
             let storyResult = get(path);
             let storyJson =
               storyResult
               >>= (
                 result =>
                   switch (result) {
                   | Error(ex) => raise(ex)
                   | Ok(json) =>
                     let storyId = getJsonValueInt("id", json);
                     let storyTitle = getJsonValueString("title", json);
                     let storyUrl = getJsonValueString("url", json);
                     let story = {
                       id: storyId,
                       title: storyTitle,
                       url: storyUrl,
                     };
                     Deferred.return(story);
                   }
               );
             storyJson;
           })
        |> Deferred.all;
      }
    );
  topStoryList;
};

/*
 let story1 = {id: 1, title: "First story", url: "http://firststory.com"};
 let story2 = {id: 2, title: "Second story", url: "http://secondstory.com"};
 let story3 = {id: 3, title: "Third story", url: "http://thridstory.com"};
 let hackerNews = {topStory: [story1, story2, story3]};*/

let rec pet1 = {id: 1, name: "pet1", color: "red", species: Cat, owner: None}
and pet2 = {
  id: 2,
  name: "pet2",
  color: "black",
  species: Dog,
  owner: Some(alice),
}
and pet3 = {
  id: 3,
  name: "pet3",
  color: "rainbow",
  species: Others,
  owner: Some(alice),
}
and alice = {
  id: 1,
  name: "Alice",
  role: Admin,
  friends: [bob],
  pets: [pet1, pet2],
}
and bob = {id: 2, name: "Bob", role: User, friends: [alice], pets: [pet3]};

let users = [alice, bob];
let pets = [pet1, pet2, pet3];

let role =
  Schema.(
    enum(
      "role",
      ~values=[
        enum_value("USer", ~value=User, ~doc="A regular user"),
        enum_value("ADmin", ~value=Admin, ~doc="An admin user"),
      ],
    )
  );

let species =
  Schema.(
    enum(
      "species",
      ~values=[
        enum_value("Cat-meao", ~value=Cat, ~doc="This is a cat"),
        enum_value("Dog-bark", ~value=Dog, ~doc="This is a dog"),
        enum_value("Bird-twick", ~value=Bird, ~doc="This is a bird"),
        enum_value(
          "Others-hmmm",
          ~value=Others,
          ~doc="This is some kind of creature",
        ),
      ],
    )
  );

let story =
  Schema.(
    obj("Story", ~fields=_storyType =>
      [
        field("id", ~args=Arg.([]), ~typ=non_null(int), ~resolve=((), p) =>
          p.id
        ),
        field(
          "title", ~args=Arg.([]), ~typ=non_null(string), ~resolve=((), p) =>
          p.title
        ),
        field("url", ~args=Arg.([]), ~typ=non_null(string), ~resolve=((), p) =>
          p.url
        ),
      ]
    )
  );

let hackernews =
  Schema.(
    obj("hackerNews", ~fields=_newsGqlType =>
      [
        io_field(
          "topStory",
          ~args=Arg.([]),
          ~typ=non_null(list(non_null(story))),
          ~resolve=((), ()) => {
            /*Call when only filed is required*/
            let ioListStory = getTopStoryList();
            ioListStory >>= (stories => Deferred.return(Ok(stories)));
            /*            p.topStory >>= (topStories => Deferred.return(Ok(topStories)));*/
          },
        ),
      ]
    )
  );

let rec user =
  lazy
    Schema.(
      obj("User", ~fields=userGqlType =>
        [
          field(
            "id", ~args=Arg.([]), ~typ=non_null(int), ~resolve=((), p: user) =>
            p.id
          ),
          field(
            "name",
            ~args=Arg.([]),
            ~typ=non_null(string),
            ~resolve=((), p: user) =>
            p.name
          ),
          field(
            "role",
            ~args=Arg.([]),
            ~typ=non_null(role),
            ~resolve=((), p: user) =>
            p.role
          ),
          field(
            "friends",
            ~args=Arg.([]),
            ~typ=list(non_null(userGqlType)),
            ~resolve=((), p) =>
            Some(p.friends)
          ),
          field(
            "pets",
            ~args=Arg.([]),
            ~typ=list(non_null(Lazy.(force(pet)))),
            ~resolve=((), p: user) =>
            Some(p.pets)
          ),
        ]
      )
    )
and pet =
  lazy
    Schema.(
      obj("pet", ~fields=_pet =>
        [
          field(
            "id", ~args=Arg.([]), ~typ=non_null(int), ~resolve=((), p: pet) =>
            p.id
          ),
          field(
            "name",
            ~args=Arg.([]),
            ~typ=non_null(string),
            ~resolve=((), p: pet) =>
            p.name
          ),
          field(
            "color", ~args=Arg.([]), ~typ=non_null(string), ~resolve=((), p) =>
            p.color
          ),
          field(
            "species",
            ~args=Arg.([]),
            ~typ=non_null(species),
            ~resolve=((), p) =>
            p.species
          ),
          field(
            /*this field can be null*/
            "owner",
            ~args=Arg.([]),
            ~typ=Lazy.(force(user)),
            ~resolve=((), p: pet) =>
            p.owner
          ),
        ]
      )
    );

let schema =
  Schema.(
    schema([
      io_field(
        "users",
        ~args=Arg.([]),
        ~typ=non_null(list(non_null(Lazy.(force(user))))),
        ~resolve=((), ()) =>
        Deferred.return(Ok(users))
      ),
      field(
        "pets",
        ~args=Arg.([]),
        ~typ=non_null(list(non_null(Lazy.(force(pet))))),
        ~resolve=((), ()) =>
        pets
      ),
      field(
        "hackerNews",
        ~args=Arg.([]),
        ~typ=non_null(hackernews),
        ~resolve=((), ()) =>
        ()
      ),
      field(
        "greeter",
        ~typ=string,
        ~args=
          Arg.[
            arg(
              "config",
              ~typ=
                non_null(
                  obj(
                    "greeter_config",
                    ~coerce=(greeting, name) => (greeting, name),
                    ~fields=[
                      arg'("greeting", ~typ=string, ~default="hello"),
                      arg("name", ~typ=non_null(string)),
                    ],
                  ),
                ),
            ),
          ],
        ~resolve=((), (), (greeting, name)) =>
        Some(Format.sprintf("%s, %s", greeting, name))
      ),
      field(
        "argTest",
        ~typ=string,
        ~args=
          Arg.[
            arg("name", ~typ=non_null(string)),
            arg("id", ~typ=non_null(string)),
          ],
        ~resolve=((), (), name, id) =>
        Some(name ++ id)
      ),
    ])
  );
