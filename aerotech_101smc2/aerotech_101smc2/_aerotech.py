__all__ = ["Aerotech"]

import asyncio
from typing import Dict, Any, List

from yaqd_core import ContinuousHardware, aserial


class Aerotech(ContinuousHardware):
    _kind = "Aerotech_101smc2"
    traits: List[str] = []
    defaults: Dict[str, Any] = {"baud_rate": 57600}

    def __init__(self, name, config, config_filepath):
        super().__init__(name, config, config_filepath)
        
        self._serial_port = aserial.ASerial(config["serial_port"], config["baud_rate"])
    ...
        # perfom other setup, possibly including reads and writes

    def _load_state(self, state):
        """Load an initial state from a dictionary (typically read from the state.toml file).

        Must be tolerant of missing fields, including entirely empty initial states.

        Parameters
        ----------
        state: dict
            The saved state to load.
        """
        super()._load_state(state)
        # This is an example to show the symetry between load and get
        # If no persistent state is needed, these unctions can be deleted
        self.value = state.get("value", 0)

    def get_state(self):
        state = super().get_state()
        state["value"] = self.value
        return state

    def _set_position(self, position):
        ...

    def close(self):
        self._serial_port.close()

    def direct_serial_write(self, message):
        self._busy = True
        self._serial_port.write(message.encode())

    async def update_state(self):
        while True:
            self._serial_port.write(b"get_status")
            line = await self._serial_port.areadline()
            self._busy = line != b"ready"
        if self._busy:
            await asyncio.sleep(0.1)
        else:
            await self._busy_sig.wait()