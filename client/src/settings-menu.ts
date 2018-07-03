import * as $ from 'jquery';
import "jquery-ui/ui/widgets/button";
import "jquery-ui/ui/widgets/menu";

export class SettingsMenu {
    public static readonly animationDuration = 100;

    public constructor() {
        $(".menu").menu();

        $("#settings-menu-button").click(() => {
            this.toggle();
        });

        $("#change-quality").click(() => {
            this.show("quality-settings-menu");
        });

        $("#advanced").click(() => {
            this.hide(() => {
                $("#advanced-settings-overlay").dialog("open");
                $("#advanced-settings-overlay-tabs").tabs({
                    heightStyle: "fill"
                });
            });
        });

        $("#settings-menu").menu();
        $("#quality-settings-menu").menu();
    }

    public toggle() {
        if (this.isOpen()) {
            this.hide();
        } else {
            this.show();
        }
    }

    public isOpen() {
        return $(".settings-menu:visible").length > 0;
    }

    public hide(callback?: () => void) {
        if (this.isOpen()) {
            $(".settings-menu:visible").hide(SettingsMenu.animationDuration, callback);
        } else {
            callback();
        }
    }

    public show(menuId?: string) {
        this.hide(() => {
            $("#" + (menuId || "settings-menu")).show(SettingsMenu.animationDuration);
        });
    }
}