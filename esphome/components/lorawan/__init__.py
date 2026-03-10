from esphome import pins
import esphome.codegen as cg
from esphome.components import spi
import esphome.config_validation as cv
from esphome.const import CONF_ID

CODEOWNERS = ["@jgc234"]
DEPENDENCIES = ["spi"]

lorawan_ns = cg.esphome_ns.namespace("lorawan")
LoRaWANComponent = lorawan_ns.class_("LoRaWANComponent", cg.Component, spi.SPIDevice)
BandList = lorawan_ns.enum("LoRaWANBandNum_t")

LORAWAN_BAND_INDEX_MAP = {
    "EU868": BandList.BandEU868,
    "US915": BandList.BandUS915,
    "EU433": BandList.BandEU433,
    "AU915": BandList.BandAU915,
    "CN470": BandList.BandCN470,
    "AS923": BandList.BandAS923,
    "AS923_2": BandList.BandAS923_2,
    "AS923_3": BandList.BandAS923_3,
    "AS923_4": BandList.BandAS923_4,
    "KR920": BandList.BandKR920,
    "IN865": BandList.BandIN865,
}

CONF_CHIPSET = "chipset"
CONF_CS_PIN = "cs_pin"
CONF_RESET_PIN = "reset_pin"
CONF_BUSY_PIN = "busy_pin"
CONF_DIO1_PIN = "dio1_pin"
CONF_BAND = "band"
CONF_SUBBAND = "sub_band"
CONF_JOIN_EUI = "join_eui"
CONF_DEV_EUI = "dev_eui"
CONF_APP_KEY = "app_key"
CONF_NWK_KEY = "nwk_key"

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(LoRaWANComponent),
            cv.Required(CONF_CHIPSET): cv.string,
            cv.Required(CONF_CS_PIN): pins.gpio_output_pin_schema,
            cv.Required(CONF_RESET_PIN): pins.gpio_output_pin_schema,
            cv.Required(CONF_BUSY_PIN): pins.gpio_input_pin_schema,
            cv.Required(CONF_DIO1_PIN): pins.gpio_input_pin_schema,
            cv.Required(CONF_BAND): cv.enum(LORAWAN_BAND_INDEX_MAP),
            cv.Optional(CONF_SUBBAND, default=1): cv.positive_not_null_int,
            cv.Required(CONF_JOIN_EUI): cv.hex_int_range(
                min=0x0000000000000000, max=0xFFFFFFFFFFFFFFFF
            ),
            cv.Required(CONF_DEV_EUI): cv.hex_int_range(
                min=0x0000000000000000, max=0xFFFFFFFFFFFFFFFF
            ),
            cv.Required(CONF_APP_KEY): cv.string,
            cv.Required(CONF_NWK_KEY): cv.string,
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(spi.spi_device_schema(cs_pin_required=True))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await spi.register_spi_device(var, config)

    cg.add(var.set_chipset(config[CONF_CHIPSET]))
    cs_pin = await cg.gpio_pin_expression(config[CONF_CS_PIN])
    cg.add(var.set_cs_pin(cs_pin))
    reset_pin = await cg.gpio_pin_expression(config[CONF_RESET_PIN])
    cg.add(var.set_reset_pin(reset_pin))
    busy_pin = await cg.gpio_pin_expression(config[CONF_BUSY_PIN])
    cg.add(var.set_busy_pin(busy_pin))
    dio1_pin = await cg.gpio_pin_expression(config[CONF_DIO1_PIN])
    cg.add(var.set_dio1_pin(dio1_pin))
    cg.add(var.set_band(config[CONF_BAND]))
    cg.add(var.set_sub_band(config[CONF_SUBBAND]))
    cg.add(var.set_join_eui(config[CONF_JOIN_EUI]))
    cg.add(var.set_dev_eui(config[CONF_DEV_EUI]))
    cg.add(var.set_app_key(config[CONF_APP_KEY]))
    cg.add(var.set_nwk_key(config[CONF_NWK_KEY]))

    cg.add_library("jgromes/RadioLib", "7.6.0")
