#include "gtest/gtest.h"
#include "fsst/main/fsst.h"
#include <fmt/core.h>
#include <fmt/format.h>
#include <filesystem>
#include <fstream>


namespace fs = std::filesystem;


TEST (SymbolTable, Build) {
    std::string data = "http://www.google.ca http://www.whatsapp.com http://www.amazon.in";

    fsst::SymbolTable st;
    fsst::BuildSymbolTable(st, (uint8_t*) data.data(), data.size());

    auto out = std::make_unique<uint8_t[]>(data.size() * 2);
    auto lenAfterEncode = fsst::Encode((uint8_t*)data.data(), data.size(), out.get(), st);
    fmt::print("lenAfterEncode={}\n", lenAfterEncode);

    auto raw = std::make_unique<uint8_t[]>(data.size() + 8);
    auto lenAfterDecode = fsst::Decode(out.get(), lenAfterEncode,
                                       raw.get(), st.Get64Symbols(), st.GetLens());
    fmt::print("lenAfterDecode={}\n", lenAfterDecode);
    auto result = std::string{(char *)raw.get(), lenAfterDecode};
    fmt::print("raw string = {}\n", result);

    ASSERT_EQ(data, result);
}


TEST (SymbolTable, LongBuild) {
    std::string data = "stevenh@xsmail.com\n"
                       "imquy25beh@nqty14ad.com\n"
                       "fotograf@aps-studio.eu\n"
                       "kmacy_gurl@hotmail.com\n"
                       "sizzling_devil@yahoo.com\n"
                       "abepe@iprimus.com.au\n"
                       "darkmen2349@hotmail.com\n"
                       "shannonzei@yahoo.com\n"
                       "luks.loku@hotmail.com\n"
                       "flecktest2@gmail.com\n"
                       "mil00813@adobe.com\n"
                       "erossimonostan@yahoo.it\n"
                       "onefuhs@ameritech.net\n"
                       "ab.81@web.de\n"
                       "gathigo.mugo@yahoo.com\n"
                       "c_p366@hotmai.com\n"
                       "florencia-romero@live.com.ar\n"
                       "hoffiesviljoen@hotmail.com\n"
                       "atholl@live.com\n"
                       "adobe@demaan.be\n"
                       "rathkatie@gmail.com\n"
                       "ohgun9212@gmail.com\n"
                       "carolinesteranka@hotmail.com\n"
                       "julia_vc85@hotmail.com\n"
                       "ben@thedunnys.wanadoo.co.uk\n"
                       "imerriem2000@hotmail.com\n"
                       "deathstrike85@hotmail.com\n"
                       "aksuyek.emir@gmail.com\n"
                       "marapa666@hotmail.com\n"
                       "ressamabdullah@hotmail.com.tr\n"
                       "ross.sarah@btinternet.com\n"
                       "theo-gruen@web.de\n"
                       "hgk090@gmail.com\n"
                       "jroper@haslingdenhigh.com\n"
                       "ta0am@hotmail.com\n"
                       "roadkill87@msn.com\n"
                       "mgopnzalez@grupomayer.com.mx\n"
                       "bk.lilnicemalufet@uyahoo.com\n"
                       "leftie@libero.it\n"
                       "kevincasaux@gmail.com\n"
                       "ghreize_021@yahoo.com\n"
                       "matt.montpas@gmail.com\n"
                       "electricfox317+ad@gmail.com\n"
                       "cumidz7@inbox.lv\n"
                       "junekim@twe.net.au\n"
                       "ramie_092408@yahoo.com\n"
                       "ilove02tinkerbell02@yahoo.com\n"
                       "ratnabharatia@niit.com\n"
                       "litsaexqpaper@aol.com\n"
                       "hoytman2015@yahoo.com\n"
                       "mitita@castrobravo.com\n"
                       "elizabeth_vb@hotmail.com\n"
                       "sigridt@hotmail.com\n"
                       "spike@spike.org.uk\n"
                       "nukeoriginal@gmail.com\n"
                       "info@clubtq.com.ar\n"
                       "b.mecklem@bigpond.com\n"
                       "karma_adina@yahoo.com\n"
                       "lyndonroderick@gmail.com\n"
                       "pinkmelons@gmail.com\n"
                       "kathtearney@hotmail.co.uk\n"
                       "valentinmaugrez94@hotmail.fr\n"
                       "aksquest@yahoo.com\n"
                       "dardobazanpas@hotmail.com\n"
                       "leohemmes@orange.nl\n"
                       "asialopocka@gmail.com\n"
                       "lariissa_@hotmail.com\n"
                       "brigida.rivordo@terra.es\n"
                       "017624289093@o2online.de\n"
                       "wugking@hotmail.com\n"
                       "simone.riess@kalypsomedia.com\n"
                       "uk-pandey@hotmail.com\n"
                       "ajamison@temple.edu\n"
                       "samjackson1987@live.co.uk\n"
                       "alex_isps_05@hotmail.com\n"
                       "lauraminini@alice.it\n"
                       "kohno@comterose.jp\n"
                       "wells_angel@ymail.com\n"
                       "asnoble@shaw.ca\n"
                       "bellagives69a@hotmail.com\n"
                       "kln.contact.multi@gmail.com\n"
                       "laurent.tran@gmail.com\n"
                       "plowkids@embarqmail.com\n"
                       "angief030@gmail.com\n"
                       "saara.oinonen@hotmail.com\n"
                       "mizz_jewsbury_x@hotmail.co.uk\n"
                       "ooisweehin@yahoo.com\n"
                       "mother_leopard@hotmail.com\n"
                       "jhs.ilha@hotmail.com\n"
                       "sparacda33@yahoo.com\n"
                       "kinko_sangyo@yahoo.co.jp\n"
                       "snoop_kid@hotmail.com\n"
                       "b200909f00060011@sbb.ne.jp\n"
                       "bandb@effect.net.au\n"
                       "keyn0@hotmail.de\n"
                       "marcelklaassen@web.de\n"
                       "jochenporemski@web.de\n"
                       "edpbsl@yahoo.com\n"
                       "georgs1@o2.pl\n"
                       "smeeker68@buckeye-express.com";

    fsst::SymbolTable st;
    fsst::BuildSymbolTable(st, (uint8_t*) data.data(), data.size());

    auto out = std::make_unique<uint8_t[]>(data.size() * 2);
    auto lenAfterEncode = fsst::Encode((uint8_t*)data.data(), data.size(), out.get(), st);
    fmt::print("rawDataSize={}, lenAfterEncode={}\n", data.size(), lenAfterEncode);

    auto raw = std::make_unique<uint8_t[]>(data.size() * 2);
    auto lenAfterDecode = fsst::Decode(out.get(), lenAfterEncode,
                                       raw.get(), st.Get64Symbols(), st.GetLens());
    fmt::print("lenAfterDecode={}\n", lenAfterDecode);
    ASSERT_EQ(data.size(), lenAfterDecode);
    ASSERT_EQ(0, ::memcmp(data.data(), raw.get(), lenAfterDecode));
}



int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}