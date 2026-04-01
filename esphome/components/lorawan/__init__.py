from esphome import pins
import esphome.codegen as cg
from esphome.components import spi
import esphome.config_validation as cv
from esphome.const import CONF_ID

CODEOWNERS = ["@jgc234"]

lorawan_ns = cg.esphome_ns.namespace("lorawan")
global_ns = cg.global_ns
LoRaWANComponent = lorawan_ns.class_("LoRaWANComponent", cg.Component)
BandList = global_ns.enum("LoRaWANBandNum_t")
Chipset_t = lorawan_ns.enum("Chipset_t")
lora_bn_ns = global_ns.namespace("LoRaWANBandNum_t")
LoRaWANBandNum_t = lora_bn_ns.enum("LoRaWANBandNum_t")

LORAWAN_CHIPSET_INDEX_MAP = {
    "SX1262": Chipset_t.Chipset_SX1262,
    "SX1268": Chipset_t.Chipset_SX1268,
}

LORAWAN_BAND_INDEX_MAP = {
    "EU868": LoRaWANBandNum_t.BandEU868,
    "US915": LoRaWANBandNum_t.BandUS915,
    "EU433": LoRaWANBandNum_t.BandEU433,
    "AU915": LoRaWANBandNum_t.BandAU915,
    "CN470": LoRaWANBandNum_t.BandCN470,
    "AS923": LoRaWANBandNum_t.BandAS923,
    "AS923_2": LoRaWANBandNum_t.BandAS923_2,
    "AS923_3": LoRaWANBandNum_t.BandAS923_3,
    "AS923_4": LoRaWANBandNum_t.BandAS923_4,
    "KR920": LoRaWANBandNum_t.BandKR920,
    "IN865": LoRaWANBandNum_t.BandIN865,
}

CONF_CHIPSET = "chipset"
CONF_CLK_PIN = "clk_pin"
CONF_MISO_PIN = "miso_pin"
CONF_MOSI_PIN = "mosi_pin"
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
            cv.Required(CONF_CHIPSET): cv.enum(LORAWAN_CHIPSET_INDEX_MAP),
            cv.Required(CONF_CLK_PIN): pins.internal_gpio_output_pin_schema,
            cv.Required(CONF_MISO_PIN): pins.internal_gpio_input_pin_schema,
            cv.Required(CONF_MOSI_PIN): pins.internal_gpio_output_pin_schema,
            cv.Required(CONF_RESET_PIN): pins.internal_gpio_output_pin_schema,
            cv.Required(CONF_BUSY_PIN): pins.internal_gpio_input_pin_schema,
            cv.Required(CONF_DIO1_PIN): pins.internal_gpio_input_pin_schema,
            cv.Required(CONF_BAND): cv.enum(LORAWAN_BAND_INDEX_MAP),
            cv.Optional(CONF_SUBBAND, default=1): cv.positive_not_null_int,
            cv.Required(CONF_JOIN_EUI): cv.hex_uint64_t,
            cv.Required(CONF_DEV_EUI): cv.hex_uint64_t,
            cv.Required(CONF_APP_KEY): cv.All(
                cv.ensure_list(cv.hex_uint8_t), cv.Length(min=16, max=16)
            ),
            cv.Required(CONF_NWK_KEY): cv.All(
                cv.ensure_list(cv.hex_uint8_t), cv.Length(min=16, max=16)
            ),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(spi.spi_device_schema(cs_pin_required=True))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await spi.register_spi_device(var, config)

    #    cg.add(var.set_chipset(config[CONF_CHIPSET]))
    reset_pin = await cg.gpio_pin_expression(config[CONF_RESET_PIN])
    cg.add(var.set_reset_pin(reset_pin))
    busy_pin = await cg.gpio_pin_expression(config[CONF_BUSY_PIN])
    cg.add(var.set_busy_pin(busy_pin))
    clk_pin = await cg.gpio_pin_expression(config[CONF_CLK_PIN])
    cg.add(var.set_clk_pin(clk_pin))
    miso_pin = await cg.gpio_pin_expression(config[CONF_MISO_PIN])
    cg.add(var.set_miso_pin(miso_pin))
    mosi_pin = await cg.gpio_pin_expression(config[CONF_MOSI_PIN])
    cg.add(var.set_mosi_pin(mosi_pin))
    dio1_pin = await cg.gpio_pin_expression(config[CONF_DIO1_PIN])
    cg.add(var.set_dio1_pin(dio1_pin))
    cg.add(var.set_chipset(config[CONF_CHIPSET]))
    cg.add(var.set_band(config[CONF_BAND]))
    cg.add(var.set_sub_band(config[CONF_SUBBAND]))
    cg.add(var.set_join_eui(config[CONF_JOIN_EUI]))
    cg.add(var.set_dev_eui(config[CONF_DEV_EUI]))
    cg.add(var.set_app_key(cg.ArrayInitializer(*config[CONF_APP_KEY])))
    cg.add(var.set_nwk_key(cg.ArrayInitializer(*config[CONF_NWK_KEY])))

    cg.add_library("jgromes/RadioLib", "7.6.0")
