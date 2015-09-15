var State = {
  OFF : {name : "Off", value : 2, code : "off"},
  HOME : {name : "Home", value : 4, code : "home"},
  AWAY : {name : "Away", value : 5, code : "away"},
};

//callback(success, responseText)
var xhrPost = function (url, params, callback) {
  var xhr = new XMLHttpRequest();
  xhr.ontimeout = function () {
    callback(false);
  };
  xhr.onload = function () {
    callback(true, this.responseText);
  };
  xhr.open('POST',url,true);
  xhr.setRequestHeader("Content-type","application/x-www-form-urlencoded");
  xhr.send(params);
};

function getStatus(statusString) {
  var status = 0;
  var state;
  
  if (statusString == State.OFF.name) {
    state = State.OFF;
  } else if (statusString == State.HOME.name) {
    state = State.HOME;
  } else if (statusString == State.AWAY.name) {
    state = State.AWAY;
  }
  
  if (state) {
    status = state.value;
  }
  
  return status;
}

function getStatusCode(status) {
  var code;
  var state;
  
  if (status == State.OFF.value) {
    state = State.OFF;
  } else if (status == State.HOME.value) {
    state = State.HOME;
  } else if (status == State.AWAY.value) {
    state = State.AWAY;
  }
  
  if (state) {
    code = state.code;
  }
  
  return code;
}

//callback(success)
function getLocations(uid, callback) {
  var url = 'https://simplisafe.com/mobile/' + uid + '/locations';
  var params = 'no_persist=1&XDEBUG_SESSION_START=session_name';

  xhrPost(url, params, 
    function(success, responseText) {
      if (success) {
        console.log(responseText);
        
        var response = JSON.parse(responseText);
        var locationId = 0;
        var statusString;
        var city;
        var street;
        
        for (locationId in response.locations) {
          console.log('location ID = ' + locationId);
          var location = response.locations[locationId];
          
          statusString = location.system_state;
          console.log('status = ' + statusString);
          
          city = location.city;
          console.log('city = ' + city);
          
          street = location.street1;
          console.log('street = ' + street);
        }
        
        var status = getStatus(statusString);
        
        // Assemble dictionary using our keys
        var dictionary = {
          'KEY_STATUS': status,
          'KEY_CITY' : city,
          'KEY_STREET' : street
        };
  
        // Send to Pebble
        Pebble.sendAppMessage(dictionary,
                              function(e) {
                                console.log('Status sent to Pebble successfully!');
                                
                                callback(true);
                              },
                              function(e) {
                                console.log('Error sending status to Pebble!');
                                
                                callback(false);
                              }
                             );
      }
    }      
  );
}

//callback(success)
function logOut(callback) {
  var url = 'https://simplisafe.com/mobile/logout';
  var params = 'no_persist=1&XDEBUG_SESSION_START=session_name';

  xhrPost(url, params, 
    function(success, responseText) {
      if (success) {
        console.log(responseText);
        
        var response = JSON.parse(responseText);
        
        var returnCode = response.return_code;
        
        if (returnCode == 1) {
          console.log('Successfully logged out.');
          
          callback(true);
        } else {
          console.log('Log out failed.');
          
          callback(false);
        }
      }
    }      
  );
}

//callback(success, uid)
function logIn(email, password, uuid, callback) {
  var url = 'https://simplisafe.com/mobile/login/';
  var params = 'name=' + email + '&pass=' + password + '&device_name=pebble&device_uuid=' + uuid + '&version=1200&no_persist=1&XDEBUG_SESSION_START=session_name';

  xhrPost(url, params, 
    function(success, responseText) {
      if (success) {
        console.log(responseText);
        
        var response = JSON.parse(responseText);
        
        var returnCode = response.return_code;
        
        if (returnCode == 1) {
          console.log('Successfully logged in.');
          
          callback(true, response.uid);
        } else {
          console.log('Log in failed.');
          
          callback(false);
        }
      }
    }      
  );
}

//callback(success, state)
function setState(uid, locationId, state, callback) {
  var url = 'https://simplisafe.com/mobile/' + uid + '/sid/' + locationId + '/set-state';
  var code = getStatusCode(state);
  var params = 'state=' + code + '&mobile=1&no_persist=1&XDEBUG_SESSION_START=session_name';

  xhrPost(url, params, 
    function(success, responseText) {
      if (success) {
        console.log(responseText);
        
        var response = JSON.parse(responseText);
        
        var result = response.result;
        
        if (state == result) {
          console.log('Successfully changed state.');
          
          callback(true, state);
        } else {
          console.log('State change failed.');
          
          callback(false);
        }
      }
    }      
  );
}

// Listen for when the app is opened
Pebble.addEventListener('ready', 
  function(e) {
    console.log('PebbleKit JS ready!');
    
    var email = 'email';
    var password = 'password';
    var uuid = '47b18807-fe9b-44f9-bd37-eac58f4e9745';
    
    logIn(email, password, uuid,
         function(success, uid) {
           if (success) {
             getLocations(uid,
                         function(success) {
                           logOut(function(success) {
                             
                           });
                         });
           }
         });
  }
);

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    console.log('AppMessage received!');
    
    // Assemble dictionary using our keys
        var dictionary = {
          'KEY_STATUS': e.payload.KEY_STATUS
        };
  
        // Send to Pebble
        Pebble.sendAppMessage(dictionary,
                              function(e) {
                                console.log('Status sent to Pebble successfully!');
                              },
                              function(e) {
                                console.log('Error sending status to Pebble!');
                              }
                             );
  }                     
);
