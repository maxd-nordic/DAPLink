from pyocd.target.family.target_nRF91 import ModemUpdater

def did_connect():
    ModemUpdater(session).verify("/home/maxd/Downloads/mfw_nrf9160_1.3.4.zip")
