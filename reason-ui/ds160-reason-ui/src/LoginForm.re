type state = {
  username: option(string),
  password: option(string),
};

type action =
  | UsernameChange(string)
  | PasswordChange(string);

let component = ReasonReact.reducerComponent("LoginForm");

let make = _children => {
  ...component,
  initialState: () => {username: None, password: None},
  reducer: (action, state) =>
    switch (action) {
    | UsernameChange(newUsername) =>
      ReasonReact.Update({...state, username: Some(newUsername)})
    | PasswordChange(newPassword) =>
      ReasonReact.Update({...state, password: Some(newPassword)})
    },
  render: self =>
    <form>
      <div className="form-group">
        <input
          className="form-control"
          id="login-username"
          placeholder="Username"
          onChange={
            event =>
              self.send(
                UsernameChange(ReactEvent.Form.target(event)##value),
              )
          }
        />
      </div>
      <div className="form-group">
        <input
          className="form-control"
          id="login-password"
          placeholder="Password"
          onChange={
            event =>
              self.send(
                PasswordChange(ReactEvent.Form.target(event)##value),
              )
          }
        />
      </div>
      <LoginMutation
        username={
          switch (self.state.username) {
          | Some(username) => username
          | None => ""
          }
        }
        password={
          switch (self.state.password) {
          | Some(password) => password
          | None => ""
          }
        }
      />
    </form>,
};
