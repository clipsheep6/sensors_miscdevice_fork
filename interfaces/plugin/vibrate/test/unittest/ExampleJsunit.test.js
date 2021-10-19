/*
 * Copyright (C) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
import vibrator from '@ohos.vibrator'

import {describe, beforeAll, beforeEach, afterEach, afterAll, it, expect} from 'deccjsunit/index'

describe("VibratorJsTest", function () {
    beforeAll(function() {
        /*
         * @tc.setup: setup invoked before all testcases
         */
         console.info('beforeAll caled')
    })
    
    afterAll(function() {
        /*
         * @tc.teardown: teardown invoked after all testcases
         */
         console.info('afterAll caled')
    })
    
    beforeEach(function() {
        /*
         * @tc.setup: setup invoked before each testcases
         */
         console.info('beforeEach caled')
    })
    
    afterEach(function() {
        /*
         * @tc.teardown: teardown invoked after each testcases
         */
         console.info('afterEach caled')
    })

    /*
     * @tc.name:VibratorJsTest001
     * @tc.desc:verify app info is not null
     * @tc.type: FUNC
     * @tc.require: Issue Number
     */
    it("VibratorJsTest001", 0, async function (done) {
        function vibrateCallback(error) {
            if (error) {
                console.info('VibratorJsTest001 HYH_TEST on error');
                expect(false).assertTrue();
            } else {
                console.info('VibratorJsTest001 HYH_TEST on success');
                expect(true).assertTrue();
            }
            done();
        }
        vibrator.vibrate(10, vibrateCallback);
    })

    /*
     * @tc.name:VibratorJsTest002
     * @tc.desc:verify app info is not null
     * @tc.type: FUNC
     * @tc.require: Issue Number
     */
    it("VibratorJsTest002", 0, async function (done) {
        function vibrateCallback(error) {
            if (error) {
                console.info('VibratorJsTest001 HYH_TEST on error');
                expect(true).assertTrue();
            } else {
                console.info('VibratorJsTest001 HYH_TEST on success');
                expect(false).assertTrue();
            }
            done();
        }
        vibrator.vibrate(-1, vibrateCallback);
    })

    /*
     * @tc.name:VibratorJsTest003
     * @tc.desc:verify app info is not null
     * @tc.type: FUNC
     * @tc.require: Issue Number
     */
    it("VibratorJsTest003", 0, async function (done) {
        function vibrateCallback(error) {
            if (error) {
                console.info('VibratorJsTest001 HYH_TEST on error');
                expect(true).assertTrue();
            } else {
                console.info('VibratorJsTest001 HYH_TEST on success');
                expect(false).assertTrue();
            }
            done();
        }
        vibrator.vibrate(1000000000000, vibrateCallback);
    })

    /*
     * @tc.name:VibratorJsTest004
     * @tc.desc:verify app info is not null
     * @tc.type: FUNC
     * @tc.require: Issue Number
     */
    it("VibratorJsTest004", 0, async function (done) {
        function vibrateCallback(error) {
            if (error) {
                console.info('VibratorJsTest001 HYH_TEST on error');
                expect(true).assertTrue();
            } else {
                console.info('VibratorJsTest001 HYH_TEST on success');
                expect(false).assertTrue();
            }
            done();
        }
        vibrator.vibrate(10, vibrateCallback, 10);
    })

    /*
     * @tc.name:VibratorJsTest005
     * @tc.desc:verify app info is not null
     * @tc.type: FUNC
     * @tc.require: Issue Number
     */
    it("VibratorJsTest005", 0, async function (done) {
        function vibrateCallback(error) {
            if (error) {
                console.info('VibratorJsTest001 HYH_TEST on error');
                expect(true).assertTrue();
            } else {
                console.info('VibratorJsTest001 HYH_TEST on success');
                expect(false).assertTrue();
            }
            done();
        }
        vibrator.vibrate("", vibrateCallback);
    })

    /*
     * @tc.name:VibratorJsTest006
     * @tc.desc:verify app info is not null
     * @tc.type: FUNC
     * @tc.require: Issue Number
     */
    it("VibratorJsTest006", 0, async function (done) {
        function vibrateCallback(error) {
            if (error) {
                console.info('VibratorJsTest001 HYH_TEST on error');
                expect(true).assertTrue();
            } else {
                console.info('VibratorJsTest001 HYH_TEST on success');
                expect(false).assertTrue();
            }
            done();
        }
        vibrator.vibrate("", vibrateCallback);
    })

    /*
     * @tc.name:VibratorJsTest007
     * @tc.desc:verify app info is not null
     * @tc.type: FUNC
     * @tc.require: Issue Number
     */
    it("VibratorJsTest007", 0, async function (done) {
        function vibrateCallback(error) {
            if (error) {
                console.info('VibratorJsTest001 HYH_TEST on error');
                expect(false).assertTrue();
            } else {
                console.info('VibratorJsTest001 HYH_TEST on success');
                expect(true).assertTrue();
            }
            done();
        }
        vibrator.vibrate("haptic.clock.timer", vibrateCallback);
    })

    /*
     * @tc.name:VibratorJsTest008
     * @tc.desc:verify app info is not null
     * @tc.type: FUNC
     * @tc.require: Issue Number
     */
    it("VibratorJsTest008", 0, async function (done) {
        function vibrateCallback(error) {
            if (error) {
                console.info('VibratorJsTest001 HYH_TEST on error');
                expect(true).assertTrue();
            } else {
                console.info('VibratorJsTest001 HYH_TEST on success');
                expect(false).assertTrue();
            }
            done();
        }
        vibrator.vibrate("haptic.clock.timer", vibrateCallback, 5);
    })

    
    /*
     * @tc.name:VibratorJsTest009
     * @tc.desc:verify app info is not null
     * @tc.type: FUNC
     * @tc.require: Issue Number
     */
    it("VibratorJsTest009", 0, async function (done) {
        function vibrateCallback(error) {
            if (error) {
                console.info('VibratorJsTest001 HYH_TEST on error');
                expect(true).assertTrue();
            } else {
                console.info('VibratorJsTest001 HYH_TEST on success');
                expect(false).assertTrue();
            }
            done();
        }
        vibrator.vibrate("haptic.clock.timer", vibrateCallback, 5);
    })

    /*
     * @tc.name:VibratorJsTest010
     * @tc.desc:verify app info is not null
     * @tc.type: FUNC
     * @tc.require: Issue Number
     */
    it("VibratorJsTest010", 0, async function (done) {
        function vibrateCallback(error) {
            if (error) {
                console.info('VibratorJsTest001 HYH_TEST on error');
                expect(true).assertTrue();
            } else {
                console.info('VibratorJsTest001 HYH_TEST on success');
                expect(false).assertTrue();
            }
            done();
        }
        vibrator.vibrate("haptic.clock.timer", vibrateCallback, 5);
    })

    /*
     * @tc.name:VibratorJsTest011
     * @tc.desc:verify app info is not null
     * @tc.type: FUNC
     * @tc.require: Issue Number
     */
    it("VibratorJsTest011", 0, async function (done) {
        function vibrateCallback(error) {
            if (error) {
                console.info('VibratorJsTest011 HYH_TEST on error');
                expect(true).assertTrue();
            } else {
                console.info('VibratorJsTest011 HYH_TEST on success');
                expect(false).assertTrue();
            }
            done();
        }
        vibrator.stop("", vibrateCallback);
    })

    /*
     * @tc.name:VibratorJsTest012
     * @tc.desc:verify app info is not null
     * @tc.type: FUNC
     * @tc.require: Issue Number
     */
    it("VibratorJsTest012", 0, async function (done) {
        function vibrateCallback(error) {
            if (error) {
                console.info('VibratorJsTest012 HYH_TEST on error');
                expect(false).assertTrue();
            } else {
                console.info('VibratorJsTest012 HYH_TEST on success');
                expect(true).assertTrue();
            }
            done();
        }
        vibrator.stop("time", vibrateCallback);
    })

    /*
     * @tc.name:VibratorJsTest013
     * @tc.desc:verify app info is not null
     * @tc.type: FUNC
     * @tc.require: Issue Number
     */
    it("VibratorJsTest013", 0, async function (done) {
        function vibrateCallback(error) {
            if (error) {
                console.info('VibratorJsTest013 HYH_TEST on error');
                expect(false).assertTrue();
            } else {
                console.info('VibratorJsTest013 HYH_TEST on success');
                expect(true).assertTrue();
            }
            done();
        }
        vibrator.stop("preset", vibrateCallback);
    })

    /*
     * @tc.name:VibratorJsTest014
     * @tc.desc:verify app info is not null
     * @tc.type: FUNC
     * @tc.require: Issue Number
     */
    it("VibratorJsTest014", 0, async function (done) {
        function vibrateCallback(error) {
            if (error) {
                console.info('VibratorJsTest014 HYH_TEST on error');
                expect(true).assertTrue();
            } else {
                console.info('VibratorJsTest014 HYH_TEST on success');
                expect(false).assertTrue();
            }
            done();
        }
        vibrator.stop("", vibrateCallback, 5);
    })
})
