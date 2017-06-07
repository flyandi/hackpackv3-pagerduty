[![Video](https://img.youtube.com/vi/miwA-Ia_fsU/0.jpg)](https://www.youtube.com/watch?v=miwA-Ia_fsU)

# Hackpack v3 PagerDuty

Hackpack v3 is a limited-edition hardware badge fabricated for [SIGNAL 2017](https://www.twilio.com/signal), _The Developer Conference_ by [Twilio](https://twilio.com).

Onboard is a Particle Electron 3G from [Particle](https://www.particle.io/), 16 Neopixel RGB LEDs, and a 2000 mAh Lithium Ion Battery.

This project rewrites the initial firmware and adds support for PagerDuty events. It uses the new Twilio Functions as proxy between the PagerDuty Webhook and the Hackpack.

As a bonus it uses the Twilio SIM Card to receive the commands but is still fully compatible with the dedicated Particle SIM Card.

## Getting Started

Learn more about the Hackpack on the [Hackpack site](https://hackpack.cc/).  

# Trigger

There are multiple ways to trigger it. The easiest way is through the existing Particle CLI.

It also supports the new Twilio Functions and can receive directly SMS/Commands via Twilio.

# Twilio Function

Below is the code for mapping PagerDuty payloads to the Hackpack. It supports multiple Hackpack's if you have more than one to update.

```javascript
var callbackTrigger = function(completed, max, callback, msg) {

 	  if(completed >= max) {
      callback(null, msg)
    }
}

exports.handler = function(context, event, callback) {

  var _ = require('lodash');

  // parse pagerduty
  if(_.isArray(event.messages)) {

    let incidentType = event.messages[0].type;
  	let client = context.getTwilioClient();
    let sims = [/** List of SIM SID's */];
    let completed = 0;

    sims.forEach(function(sim, index) {

      client.preview.wireless.commands.create({
          command: incidentType,
          sim: sim
      }).then(function(response) {

        	completed++;

        	callbackTrigger(completed, sims.length, callback, 'triggered ' + incidentType);
      });

    });

    return;

  }

  return callback('Error: No incident type found', false);

};

};
```


## Licensing

* MIT License
