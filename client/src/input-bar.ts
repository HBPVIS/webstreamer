import * as $ from 'jquery';
import "jquery-ui/ui/widgets/button";
import "jquery-ui/ui/widgets/checkboxradio";
import { SettingsMenu } from './settings-menu';

export class InputBar {
    public onPlay: () => void;
    public onStop: () => void;
    public onAquireInput: () => void;
    public onReleaseInput: () => void;

    private settingsMenu: SettingsMenu;

    constructor() {
        this.settingsMenu = new SettingsMenu();

        let isPlaying = true;
        const fadeTime = 100;

        $("#input-bar button").button();
        $("#play-stop-button").click(() => {
            if (isPlaying) {
                if (this.onStop) {
                    this.onStop();
                }
                $("#stop-icon").fadeOut(fadeTime, () => {
                    $("#play-icon").fadeIn(fadeTime, () => {
                        isPlaying = false;
                    });
                });
            } else {
                if (this.onPlay) {
                    this.onPlay();
                }
                $("#play-icon").fadeOut(fadeTime, () => {
                    $("#stop-icon").fadeIn(fadeTime, () => {
                        isPlaying = true;
                    });
                });
            }
        });
        (<any>$("#input-button")).checkboxradio({
            icon: false
        });
        $("#input-button").prop("checked", false);
        $("#input-button").click(() => {
            if ($("#input-button").prop("checked")) {
                if (this.onAquireInput) {
                    this.onAquireInput();
                }
            } else {
                if (this.onReleaseInput) {
                    this.onReleaseInput();
                }
            }
        })
    }
}