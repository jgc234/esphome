from esphome import pins
import esphome.codegen as cg
from esphome.components import spi
import esphome.config_validation as cv
from esphome.const import CONF_ID

CODEOWNERS = ["@jgc234"]
DEPENDENCIES = ["spi"]

# LORAWAN_BANDS = {
#     "EU868": Radiolib.LORA_BAND_EU868,
#     "US915": Radiolib.LORA_BAND_US915,
#     "EU433": Radiolib.LORA_BAND_EU433,
#     "AU915": Radiolib.LORA_BAND_AU915,
#     "CN470": Radiolib.LORA_BAND_CN470,
#     "AS923": Radiolib.LORA_BAND_AS923,
#     "AS923_2": Radiolib.LORA_BAND_AS923_2,
#     "AS923_3": Radiolib.LORA_BAND_AS923_3,
#     "AS923_4": Radiolib.LORA_BAND_AS923_4,
#     "KR920": Radiolib.LORA_BAND_KR920,
#     "IN865": Radiolib.LORA_BAND_IN865,
# }

LORAWAN_ALLOWED_BANDS = [
    "EU868",
    "US915",
    "EU433",
    "AU915",
    "CN470",
    "AS923",
    "AS923_2",
    "AS923_3",
    "AS923_4",
    "KR920",
    "IN865",
]

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

lorawan_ns = cg.esphome_ns.namespace("lorawan")
LoRaWANComponent = lorawan_ns.class_("LoRaWANComponent", cg.Component, spi.SPIDevice)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(LoRaWANComponent),
            cv.Required(CONF_CHIPSET): cv.string,
            cv.Required(CONF_CS_PIN): pins.gpio_output_pin_schema,
            cv.Required(CONF_RESET_PIN): pins.gpio_output_pin_schema,
            cv.Required(CONF_BUSY_PIN): pins.gpio_output_pin_schema,
            cv.Required(CONF_DIO1_PIN): pins.gpio_output_pin_schema,
            cv.Required(CONF_BAND): cv.one_of(*LORAWAN_ALLOWED_BANDS),
            cv.Optional(CONF_SUBBAND): cv.positive_not_null_int,
            cv.Required(CONF_JOIN_EUI): cv.string,
            cv.Required(CONF_DEV_EUI): cv.string,
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
    cg.add_library("jgromes/RadioLib", "7.6.0")
